// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGPawnMovement.h"

ACGPawn::ACGPawn()
{
	UCGPieceMovementBase* moveComp = CreateDefaultSubobject<UCGPawnMovement>(TEXT("MoveValidator"));
	AddOwnedComponent(moveComp);
}

void ACGPawn::MoveToTileInternal(ACGTile* pTile, FCGUndo& undo, bool pEvents)
{
	//en passant capture
	if (EnPassantTile && EnPassantTile == pTile)
	{
		ACGTile* otherPawnTile = EnPassantTile->Neighbours[(IsBlack() ? EDir::NORTH : EDir::SOUTH)];
		if (ACGPawn* otherPawn = Cast<ACGPawn>(otherPawnTile->OccupiedBy))
		{
			otherPawn->Flags |= EPieceFlags::EnPassantCaptured;
			undo.Capture = otherPawn;
			otherPawn->Capture(pEvents);
		}
	}
	//en passant restore
	if ((Flags & EPieceFlags::EnPassantCaptured) == EPieceFlags::EnPassantCaptured)
	{
		//fix the position of this pawn
		Super::MoveToTileInternal(pTile->Neighbours[IsBlack() ? EDir::SOUTH : EDir::NORTH], undo, pEvents);
		Flags = Flags & ~EPieceFlags::EnPassantCaptured;
	}
	else
	{
		Super::MoveToTileInternal(pTile, undo, pEvents);
	}
}

void ACGPawn::ClientBeginPromotion_Implementation()
{

}

void ACGPawn::ServerEndPromotion_Implementation()
{

}