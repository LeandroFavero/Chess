// Fill out your copyright notice in the Description page of Project Settings.

#include "CGPiece.h"
#include "CGKing.h"
#include "Misc/Char.h"
#include "GameLogic/CGHighlightableComponent.h"
#include "GameLogic/CGAnimInstance.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "GameLogic/CGChessPlayerController.h"
#include "ChessLogic/CGTile.h"
#include "GameLogic/CGUndo.h"
#include "GameLogic/CGCapturedPieces.h"
#include "GameLogic/CGGameState.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/CGBPUtils.h"

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}

// Sets default values
ACGPiece::ACGPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Collision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));

	AddOwnedComponent(RootComponent);
	AddOwnedComponent(Mesh);
	AddOwnedComponent(Collision);

	SetRootComponent(RootComponent);
	Mesh->SetupAttachment(RootComponent);
	Collision->SetupAttachment(RootComponent);

	UCGHighlightableComponent* highlight = CreateDefaultSubobject<UCGHighlightableComponent>(TEXT("Highlight"));
	AddOwnedComponent(highlight);
	if (Mesh && Collision)
	{
		Mesh->SetCollisionProfileName(FName("NoCollision"), false);
		Collision->SetCollisionProfileName(FName("ChessPiece"), false);
	}

	bReplicates = true;
	bOnlyRelevantToOwner = false;
}

ACGPiece::ACGPiece(ACGChessBoard* pBoard, uint8 pFlags):ACGPiece()
{
	Board = pBoard;
	Flags = pFlags;
}

void ACGPiece::SetColor(bool isWhite)
{
	Flags &= Flags & ~EPieceFlags::IsBlack;//Clear
	Flags |= static_cast<uint8>(!isWhite) & EPieceFlags::IsBlack;//Set
	RefreshMaterial();
}

void ACGPiece::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Mesh)
	{
		Mesh->SetAnimInstanceClass(AnimTemplate);
	}
}

// Called when the game starts or when spawned
void ACGPiece::BeginPlay()
{
	Super::BeginPlay();
}

void ACGPiece::Destroyed()
{
	if (Board)
	{
		Board->Pieces.Remove(this);
	}
	Super::Destroyed();
}

void ACGPiece::RefreshMaterial()
{
	ACGGameState* state = GetWorld()->GetGameState<ACGGameState>();
	if (Mesh && state)
	{
		Mesh->SetMaterial(0, IsWhite() ? state->WhiteMaterial : state->BlackMaterial);
	}
}


void ACGPiece::ServerGrab(bool isGrabbed)
{
	SetReplicateMovement(isGrabbed);
	OnPieceGrabbed(isGrabbed);
	if (Mesh)
	{
		if (UCGAnimInstance* animBp = Cast<UCGAnimInstance>(Mesh->GetAnimInstance()))
		{
			animBp->bIsGrabbed = isGrabbed;
		}
	}
	if (!isGrabbed)
	{
		ClientSnapToPlace();
	}
}

void ACGPiece::SnapToPlace()
{
	if (Board)
	{
		ensure(!IsCaptured());
		SetActorRelativeTransform(Board->CoordToTransform(Position));
	}
}

void ACGPiece::ClientSnapToPlace_Implementation()
{
	SnapToPlace();
}

void ACGPiece::MoveToTile(ACGTile* pTile)
{
	if (!Board || !pTile)
	{
		return;
	}
	//is it my turn?
	if (Board->EnforceMoveOrder)
	{
		if (Board->Undos.Num() == 0)
		{
			if (IsBlack())
			{
				return;
			}
		}
		else if(Board->Undos.Last().LastMoveIsBlack == IsBlack())
		{
			return;
		}
	}

	TSet<ACGTile*> moves = AvailableMoves();
	if (!moves.Contains(pTile))
	{
		OnInvalidMove();
		return;
	}

	FCGUndo& undo = Board->CreateUndo();

	//can other similar piece move to the same tile? if yes we can't use simple notation
	if (ACGPiece** other = Board->Pieces.FindByPredicate([&](ACGPiece* p) {
		return p && !p->IsCaptured() && p->GetClass() == GetClass() && p != this && p->IsBlack() == IsBlack() && p->AvailableMoves().Contains(pTile);
	}))
	{
		undo.SimpleNotation = false;
	}

	MoveToTileInternal(pTile, undo, true);
	SnapToPlace();
	Flags |= EPieceFlags::Moved;
	
	//did we give check?
	if (ACGKing* k = (IsBlack() ? Board->WhiteKing : Board->BlackKing))
	{
		Board->RebuildAttackMap(IsBlack());
		undo.Check = k->IsInCheck();
	}
	undo.LastMoveIsBlack = IsBlack();

	//listen server has to update the ui
	if (UCGBPUtils::IsListenServer(this))
	{
		Board->UndoNotify();
		OnPieceMoved();
	}
	else
	{
		ClientOnPieceMoved();
	}
}

void ACGPiece::MoveToTileInternal(ACGTile* pTile, FCGUndo& undo, bool pEvents)
{
	if (!Board || !pTile)
	{
		return;
	}
	undo.From = Tile;
	undo.Piece = this;
	if(Tile)
	{
		Tile->OccupiedBy = nullptr;
	}
	Position = pTile->Position;
	Tile = pTile;
	undo.To = Tile;
	undo.Flags = Flags;
	if (Tile && Tile->OccupiedBy)
	{
		//Capture
		ACGPiece* p = Tile->OccupiedBy;
		if (p && (p->IsBlack() != IsBlack()))
		{
			undo.Capture = p;
			p->Capture(pEvents);
		}
	}
	Tile->OccupiedBy = this;
}

TSet<ACGTile*> ACGPiece::AvailableMoves()
{
	TSet<ACGTile*> ret;
	TArray<UCGPieceMovementBase*> validators;
	GetComponents<UCGPieceMovementBase>(validators);
	for (UCGPieceMovementBase* v : validators)
	{
		ensure(v);
		v->AvailableMoves(ret);
	}

	//check check
	for (auto it = ret.CreateIterator(); it; ++it)
	{
		FCGUndo undo;
		ACGTile* t = *it;
		MoveToTileInternal(t, undo, false);
		//check check
		Board->RebuildAttackMap(IsWhite());
		if (IsBlack() ? Board->BlackKing->IsInCheck() : Board->WhiteKing->IsInCheck())
		{
			it.RemoveCurrent();
		}
		Board->UndoInternal(undo);
	}
	return ret;
}

bool ACGPiece::HasAvailableMoves()
{
	//TODO: optimize
	return AvailableMoves().Num() > 0;
}

void ACGPiece::FillAttackMap()
{
	if (IsCaptured())
	{
		return;
	}
	TSet<ACGTile*> tiles;
	TArray<UCGPieceMovementBase*> validators;
	GetComponents<UCGPieceMovementBase>(validators);
	for (UCGPieceMovementBase* v : validators)
	{
		ensure(v);
		v->AttackedTiles(tiles);
	}
	for (ACGTile* t : tiles)
	{
		ensure(t);
		t->AttackedBy.Add(this);
	}
}

void ACGPiece::Capture(bool pAddToCaptured)
{
	if (!Board && !Tile)
	{
		return;
	}
	Tile->OccupiedBy = nullptr;
	Flags |= EPieceFlags::Captured;
	if (pAddToCaptured)
	{
		if (IsBlack())
		{
			if (Board->CapturedBlack)
			{
				Board->CapturedBlack->Add(this);
			}
		}
		else
		{
			if (Board->CapturedWhite)
			{
				Board->CapturedWhite->Add(this);
			}
		}
	}
}

void ACGPiece::UnCapture()
{
	if(!Board)
	{ 
		return;
	}
	Flags &= ~EPieceFlags::Captured;
	if (IsBlack())
	{
		if (Board->CapturedBlack)
		{
			Board->CapturedBlack->Remove(this);
		}
	}
	else
	{
		if (Board->CapturedWhite)
		{
			Board->CapturedWhite->Remove(this);
		}
	}
}

const bool ACGPiece::IsCaptured() const
{
	return (Flags & EPieceFlags::Captured) == EPieceFlags::Captured;
}

void ACGPiece::ClientOnPieceMoved_Implementation()
{
	OnPieceMoved();
}

void ACGPiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGPiece, Position)
	DOREPLIFETIME(ACGPiece, Board)
	DOREPLIFETIME(ACGPiece, Tile)
	DOREPLIFETIME(ACGPiece, Flags)
}