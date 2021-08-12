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

ACGPiece::ACGPiece()
{
	PrimaryActorTick.bCanEverTick = false;

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

ACGPiece::ACGPiece(ACGChessBoard* iBoard, uint8 iFlags)
	:ACGPiece()
{
	Board = iBoard;
	Flags = iFlags;
}

void ACGPiece::SetColor(bool iIsWhite)
{
	Flags &= ~EPieceFlags::IsBlack;//Clear
	if (!iIsWhite) 
	{
		Flags |= EPieceFlags::IsBlack;//Set
	}
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

void ACGPiece::Grab(bool iIsGrabbed)
{
	SetReplicateMovement(iIsGrabbed);
	if (Mesh)
	{
		if (UCGAnimInstance* animBp = Cast<UCGAnimInstance>(Mesh->GetAnimInstance()))
		{
			animBp->bIsGrabbed = iIsGrabbed;
		}
	}
	if (!iIsGrabbed)
	{
		ClientSnapToPlace();
	}
}

void ACGPiece::SnapToPlace()
{
	if (Board && !IsCaptured())
	{
		SetActorRelativeTransform(Board->CoordToTransform(Position));
	}
}

void ACGPiece::ClientSnapToPlace_Implementation()
{
	SnapToPlace();
}

void ACGPiece::MoveToTile(ACGTile* iTile)
{
	if (!Board || !iTile)
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

	TSet<ACGTile*> moves = GetAvailableMoves();
	if (!moves.Contains(iTile))
	{
		return;
	}

	FCGUndo& undo = Board->CreateUndo();

	//can other similar piece move to the same tile? if yes we can't use simple notation
	if (ACGPiece** other = Board->Pieces.FindByPredicate([&](ACGPiece* p) {
		return p && !p->IsCaptured() && p->GetClass() == GetClass() && p != this && p->IsBlack() == IsBlack() && p->GetAvailableMoves().Contains(iTile);
	}))
	{
		undo.SimpleNotation = false;
	}

	MoveToTileInternal(iTile, undo, true);
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
	if (UCGBPUtils::IsLocalUpdateRequired(this))
	{
		Board->UndoNotify();
	}
}

void ACGPiece::MoveToTileInternal(ACGTile* iTile, FCGUndo& oUndo, bool iEvents)
{
	if (!Board || !iTile)
	{
		return;
	}
	oUndo.From = Tile;
	oUndo.Piece = this;
	if(Tile)
	{
		Tile->OccupiedBy = nullptr;
	}
	Position = iTile->Position;
	Tile = iTile;
	oUndo.To = Tile;
	oUndo.Flags = Flags;
	if (Tile && Tile->OccupiedBy)
	{
		//Capture
		ACGPiece* p = Tile->OccupiedBy;
		if (p && (p->IsBlack() != IsBlack()))
		{
			oUndo.Capture = p;
			oUndo.FiftyMoveCounter = 0;
			p->Capture(iEvents);
		}
	}
	Tile->OccupiedBy = this;
}

TSet<ACGTile*> ACGPiece::GetAvailableMoves()
{
	TSet<ACGTile*> ret;
	if (IsCaptured())
	{
		return ret;
	}
	TArray<UCGPieceMovementBase*> validators;
	GetComponents<UCGPieceMovementBase>(validators);
	for (UCGPieceMovementBase* v : validators)
	{
		ensure(v);
		v->GetAvailableMoves(ret);
	}
	//check check
	if (Board && Board->BlackKing && Board->WhiteKing)
	{
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
	}
	return ret;
}

bool ACGPiece::HasAvailableMoves()
{
	//TODO: optimize
	return GetAvailableMoves().Num() > 0;
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
		v->GetAttackedTiles(tiles);
	}
	for (ACGTile* t : tiles)
	{
		ensure(t);
		t->AttackedBy.Add(this);
	}
}

void ACGPiece::Capture(bool iAddToCaptured)
{
	if (!Board && !Tile)
	{
		return;
	}
	Tile->OccupiedBy = nullptr;
	Flags |= EPieceFlags::Captured | (iAddToCaptured ? 0 : EPieceFlags::DummyCaptured);
	if (iAddToCaptured)
	{
		Position = FCGSquareCoord(0xff, 0xff);
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
	if ((Flags & EPieceFlags::DummyCaptured) != EPieceFlags::DummyCaptured)
	{
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
	Flags &= ~(EPieceFlags::Captured | EPieceFlags::DummyCaptured);
}

ACGChessPlayerController* ACGPiece::GetCGController()
{
	if (UCGBPUtils::IsHotSeatMode(this))
	{
		return GetWorld()->GetFirstPlayerController<ACGChessPlayerController>();
	}

	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if (ACGChessPlayerController* pc = Cast<ACGChessPlayerController>(*it))
		{
			if (pc->bIsBlack == IsBlack())
			{
				return pc;
			}
		}
	}
	return nullptr;
}

const bool ACGPiece::IsCaptured() const
{
	return (Flags & EPieceFlags::Captured) == EPieceFlags::Captured;
}

void ACGPiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const//Parameter cannot be renamed!
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGPiece, Position)
	DOREPLIFETIME(ACGPiece, Board)
	DOREPLIFETIME(ACGPiece, Tile)
	DOREPLIFETIME(ACGPiece, Flags)
}