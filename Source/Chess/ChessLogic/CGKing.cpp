// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGKing.h"
#include "ChessLogic/CGRook.h"
#include "CGKingMovement.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGTile.h"

ACGKing::ACGKing()
{
	UCGKingMovement* moveComp = CreateDefaultSubobject<UCGKingMovement>(TEXT("MoveValidator"));
	AddOwnedComponent(moveComp);
}

void ACGKing::MoveToTileInternal(ACGTile* pTile, FCGUndo& undo, bool pEvents)
{
	if (!pTile)
	{
		return;
	}
	if (CastleTiles.Contains(pTile))
	{
		EDir side = Position.X > pTile->Position.X ? EDir::WEST : EDir::EAST;
		EDir otherSide = Position.X > pTile->Position.X ? EDir::EAST : EDir::WEST;
		for (ACGTile* t = pTile->Neighbours[side]; t; t = t->Neighbours[side])
		{
			if (t && t->OccupiedBy)
			{
				if (ACGRook* r = Cast<ACGRook>(t->OccupiedBy))
				{
					undo.CastleRook = r;
					undo.CastleRookTile = t;
					FCGUndo dummyUndo;
					r->MoveToTileInternal(pTile->Neighbours[otherSide], dummyUndo, false);
					r->ClientSnapToPlace();
					break;
				}
			}
		}
	}
	Super::MoveToTileInternal(pTile, undo, pEvents);
}

TSet<ACGTile*> ACGKing::AvailableMoves()
{
	TSet<ACGTile*> ret;
	CastleTiles.Empty();
	Board->RebuildAttackMap(IsWhite());
	TArray<UCGPieceMovementBase*> validators;
	GetComponents<UCGPieceMovementBase>(validators);
	for (UCGPieceMovementBase* v : validators)
	{
		ensure(v);
		v->AvailableMoves(ret);
	}
	return ret;
}

bool ACGKing::IsInCheck()
{
	if (Tile)
	{
		return Tile->AttackedBy.Num() > 0;
	}
	return false;
}
