// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPieceMovementBase.h"
#include <GameLogic/CGBoardTile.h>

#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGBoardTile.h"

// Sets default values for this component's properties
UCGPieceMovementBase::UCGPieceMovementBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCGPieceMovementBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCGPieceMovementBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCGPieceMovementBase::IsMoveValid(const ACGBoardTile* pTile)
{
	if (!pTile)
	{
		return false;
	}
	TSet<ACGBoardTile*> set;
	AvailableMoves(set);
	return set.Contains(pTile);
	/*for (const ACGBoardTile* t : set)
	{
		if (t == pTile)
		{
			return true;
		}
	}
	return false;*/
}

void UCGPieceMovementBase::AvailableMoves(TSet<ACGBoardTile*>& set)
{

}

void UCGPieceMovementBase::AttackedTiles(TSet<ACGBoardTile*>& set)
{
	AvailableMoves(set);
}