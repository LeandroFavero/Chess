// Fill out your copyright notice in the Description page of Project Settings.

#include "CGPiece.h"
#include "Misc/Char.h"
#include "GameLogic/CGHighlightableComponent.h"
#include "GameLogic/CGAnimInstance.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "GameLogic/CGChessPlayerController.h"


//#define Dbg(...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, __VA_ARGS__); }

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

// Sets default values
ACGPiece::ACGPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called every frame
void ACGPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACGPiece::SetMaterial(UMaterialInstance* mat)
{
	if (Mesh)
	{
		Mesh->SetMaterial(0, mat);
	}
}


void ACGPiece::Grab(bool isGrabbed)
{
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
		SnapToPlace();
	}
}

bool ACGPiece::UpdateGrab(/*ACGChessPlayerController* pc, */FVector_NetQuantize Location)
{
	SetActorLocation(FVector(Location.X, Location.Y, 0), true);
	return true;
}

void ACGPiece::SnapToPlace()
{
	if (Board)
	{
		SetActorRelativeTransform(Board->CoordToTransform(Position));
	}
}

void ACGPiece::MoveTo(const FCGSquareCoord& coord, bool bypassCheck)
{
	if (!bypassCheck)
	{
		TArray<UCGPieceMovementBase*> validators;
		GetComponents<UCGPieceMovementBase>(validators);
		for (UCGPieceMovementBase* v : validators)
		{
			ensure(v);
			if (!v->IsMoveValid(coord))
			{
				OnInvalidMove();
				return;
			}
		}
	}

	Position = coord;

	//Tile->
	Tile = Board->GetTile(Position);

	Dbg("New position: %d,%d",coord.X, coord.Y);
	SnapToPlace();
	OnPieceMoved(coord);
}

TSet<ACGBoardTile*> ACGPiece::AvailableMoves()
{
	TSet<ACGBoardTile*> ret;
	TArray<UCGPieceMovementBase*> validators;
	GetComponents<UCGPieceMovementBase>(validators);
	for (UCGPieceMovementBase* v : validators)
	{
		ensure(v);
		v->AvailableMoves(ret);
	}
	return ret;
}

/*
void MoveTo(ACGBoardTile* tile, bool bypassCheck)
{

}

void MoveTo(ACGPiece* piece, bool bypassCheck)
{

}
*/
void ACGPiece::FillAttackMap()
{

}

void ACGPiece::Capture()
{

}

void ACGPiece::OnInvalidMove_Implementation()
{

}

void ACGPiece::OnPieceCaptured_Implementation()
{

}

void ACGPiece::OnPieceMoved_Implementation(FCGSquareCoord coord)
{

}

void ACGPiece::OnPieceSpawned_Implementation()
{

}

void ACGPiece::OnPieceGrabbed_Implementation(bool isGrabbed)
{

}