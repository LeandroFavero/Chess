// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawnMovement.h"
#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGBoardTile.h"
#include "CGChessBoard.h"


//TODO: EN PASSANT !!!
//TODO: promotion !!!

void UCGPawnMovement::AvailableMoves(TSet<ACGBoardTile*>& set)
{
	ACGPawn* pawn = GetOwner<ACGPawn>();
	if (pawn && pawn->Tile && pawn->Board)
	{
		ACGBoardTile* t = pawn->Tile->Neighbours[pawn->IsBlack() ? ACGBoardTile::SOUTH : ACGBoardTile::NORTH];
		if (t)
		{
			if (t->OccupiedBy.Num() == 0)
			{
				set.Add(t);

				//can double open?
				if ((pawn->IsBlack() && pawn->Position.Y == pawn->Board->Size.Y - 2) || (pawn->IsWhite() && pawn->Position.Y == 1))
				{
					t = t->Neighbours[pawn->IsBlack() ? ACGBoardTile::SOUTH : ACGBoardTile::NORTH];
					if (t && t->OccupiedBy.Num() == 0)
					{
						set.Add(t);
					}
				}
			}
		}
		//attack
		t = pawn->Tile->Neighbours[pawn->IsBlack() ? ACGBoardTile::SOUTH_EAST : ACGBoardTile::NORTH_EAST];
		if(t)
		{
			for (const ACGPiece* p : t->OccupiedBy)
			{
				if (p->IsBlack() != pawn->IsBlack())
				{
					set.Add(t);
					break;
				}
			}
		}
		t = pawn->Tile->Neighbours[pawn->IsBlack() ? ACGBoardTile::SOUTH_WEST : ACGBoardTile::NORTH_WEST];
		if (t)
		{
			for (const ACGPiece* p : t->OccupiedBy)
			{
				if (p->IsBlack() != pawn->IsBlack())
				{
					set.Add(t);
					break;
				}
			}
		}
	}
}

void UCGPawnMovement::AttackedTiles(TSet<ACGBoardTile*>& set)
{
	ACGPawn* owner = GetOwner<ACGPawn>();
	if (owner)
	{
		ACGBoardTile* t;
		t = owner->Tile->Neighbours[owner->IsBlack() ? ACGBoardTile::SOUTH_WEST : ACGBoardTile::NORTH_WEST];
		if (t)
		{
			set.Add(t);
		}
		t = owner->Tile->Neighbours[owner->IsBlack() ? ACGBoardTile::SOUTH_EAST : ACGBoardTile::NORTH_EAST];
		if (t)
		{
			set.Add(t);
		}
	}
}