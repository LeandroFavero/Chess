// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGChessBoard.h"
#include "GameLogic/CGBoardTile.h"
#include "ChessLogic/CGPawnMovement.h"

ACGPawn::ACGPawn()
{
	UCGPieceMovementBase* moveComp = CreateDefaultSubobject<UCGPawnMovement>(TEXT("MoveValidator"));
	AddOwnedComponent(moveComp);
}


void ACGPawn::MoveToTileInternal(ACGBoardTile* pTile, FCGUndo& undo, bool pEvents = true)
{
	//en passant capture
	if (EnPassantTile && EnPassantTile == pTile)
	{
		ACGBoardTile* otherPawnTile = EnPassantTile->Neighbours[(IsBlack() ? ACGBoardTile::NORTH : ACGBoardTile::SOUTH)];
		for (auto it = otherPawnTile->OccupiedBy.CreateIterator(); it; ++it)
		{
			ACGPawn* otherPawn = Cast<ACGPawn>(*it);
			if (otherPawn)
			{
				it.RemoveCurrent();
				otherPawn->Flags |= EPieceFlags::EnPassantCaptured;
				undo.Capture = otherPawn;
				otherPawn->Capture(pEvents);
			}
		}
	}
	//en passant restore
	if ((Flags & EPieceFlags::EnPassantCaptured) == EPieceFlags::EnPassantCaptured)
	{
		//fix the position of this pawn
		Super::MoveToTileInternal(pTile->Neighbours[IsBlack() ? ACGBoardTile::SOUTH : ACGBoardTile::NORTH], undo, pEvents);
		Flags = Flags & ~EPieceFlags::EnPassantCaptured;
	}
	else
	{
		Super::MoveToTileInternal(pTile, undo, pEvents);
	}
}

void ACGPawn::BeginPromotion()
{

}

void ACGPawn::EndPromotion()
{

}