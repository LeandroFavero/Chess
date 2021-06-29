// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawnMovement.h"
#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGBoardTile.h"

bool UCGPawnMovement::IsMoveValid(const FCGSquareCoord& coord)
{
	TSet<ACGBoardTile*> set;
	AvailableMoves(set);
	for (const ACGBoardTile* t : set)
	{
		if (t && t->Position == coord)
		{
			return true;
		}
	}
	return false;
}

void UCGPawnMovement::AvailableMoves(TSet<ACGBoardTile*>& set)
{
	ACGPawn* owner = GetOwner<ACGPawn>();
	if (owner)
	{
		ACGBoardTile* t = owner->Tile->Neighbours[owner->IsBlack ? ACGBoardTile::SOUTH : ACGBoardTile::NORTH];
		if (t)
		{
			if (t->OccupiedBy.Num() == 0)
			{
				set.Add(t);
				if (owner->IsDoubleOpenAvailable())
				{
					t = t->Neighbours[owner->IsBlack ? ACGBoardTile::SOUTH : ACGBoardTile::NORTH];
					if (t && t->OccupiedBy.Num() == 0)
					{
						set.Add(t);
					}
				}
			}
		}
		t = owner->Tile->Neighbours[owner->IsBlack ? ACGBoardTile::SOUTH_EAST : ACGBoardTile::NORTH_EAST];
		if(t)
		{
			for (const ACGPiece* p : t->OccupiedBy)
			{
				if (p->IsBlack != owner->IsBlack)
				{
					set.Add(t);
					break;
				}
			}
		}
		t = owner->Tile->Neighbours[owner->IsBlack ? ACGBoardTile::SOUTH_WEST : ACGBoardTile::NORTH_WEST];
		if (t)
		{
			for (const ACGPiece* p : t->OccupiedBy)
			{
				if (p->IsBlack != owner->IsBlack)
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
		t = owner->Tile->Neighbours[owner->IsBlack ? ACGBoardTile::SOUTH_WEST : ACGBoardTile::NORTH_WEST];
		if (t)
		{
			set.Add(t);
		}
		t = owner->Tile->Neighbours[owner->IsBlack ? ACGBoardTile::SOUTH_EAST : ACGBoardTile::NORTH_EAST];
		if (t)
		{
			set.Add(t);
		}
	}
}