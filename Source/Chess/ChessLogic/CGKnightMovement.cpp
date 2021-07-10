// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGKnightMovement.h"
#include "GameLogic/CGBoardTile.h"
#include "ChessLogic/CGPiece.h"

void UCGKnightMovement::AvailableMoves(TSet<ACGBoardTile*>& set)
{
	ACGPiece* piece = GetOwner<ACGPiece>();
	if (piece && piece->Tile)
	{
		for (int i = static_cast<int>(EDir::KNIGHT1); i <= static_cast<int>(EDir::KNIGHT8); ++i)
		{
			ACGBoardTile* t = piece->Tile->Neighbours[i];
			if (t)
			{
				for (ACGPiece* other : t->OccupiedBy)
				{
					if (piece->IsBlack() == other->IsBlack())
					{
						goto continue_directions;
					}
				}
				set.Add(t);
			}
		continue_directions:;
		}
	}
}