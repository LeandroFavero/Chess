// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGKingMovement.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGKing.h"
#include "ChessLogic/CGRook.h"
#include "ChessLogic/CGChessBoard.h"

//IMPORTANT! Attack map has to be built before calling this
void UCGKingMovement::AvailableMoves(TSet<ACGTile*>& set)
{
	ACGKing* piece = GetOwner<ACGKing>();
	if (piece && piece->Tile && piece->Board)
	{
		for (int i = EDir::NORTH; i <= EDir::NORTH_WEST; ++i)
		{
			ACGTile* t = piece->Tile->Neighbours[i];
			if (t)
			{
				if (ACGPiece* other = Cast<ACGPiece>(t->OccupiedBy))
				{
					if (piece->IsBlack() == other->IsBlack())
					{
						continue;
					}
				}
				if (t->AttackedBy.Num() == 0)//the king can only move to not attacked tiles
				{
					set.Add(t);
				}
			}
		}
		//can castle?
		if (!piece->IsMoved())
		{
			for (EDir dir : {EDir::EAST, EDir::WEST})
			{
				int distance {0};
				ACGTile* tileAtTwoDistance{ nullptr };
				for (ACGTile* t = piece->Tile->Neighbours[dir]; t; t = t->Neighbours[dir])
				{
					if (t)
					{
						if (distance < 2 && t->AttackedBy.Num() > 0)
						{
							//bail, can't move the king over attacked tiles
							break;
						}
						if (distance == 1)
						{
							tileAtTwoDistance = t;
						}
						if (t->OccupiedBy)
						{
							if (t->OccupiedBy->IsA(ACGRook::StaticClass()) && t->OccupiedBy->IsBlack() == piece->IsBlack() && !t->OccupiedBy->IsMoved())
							{
								//can castle this way
								piece->CastleTiles.Add(tileAtTwoDistance);
								set.Add(tileAtTwoDistance);
							}
							else
							{
								break;
							}
						}
						
					}
					distance += 1;
				}
			}
		}
	}
}

void UCGKingMovement::AttackedTiles(TSet<ACGTile*>& set)
{
	ACGKing* piece = GetOwner<ACGKing>();
	if (piece && piece->Tile && piece->Board)
	{
		for (int i = EDir::NORTH; i <= EDir::NORTH_WEST; ++i)
		{
			if (ACGTile* t = piece->Tile->Neighbours[i])
			{
				set.Add(t);
			}
		}
	}
}