// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGKingMovement.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGKing.h"
#include "ChessLogic/CGRook.h"
#include "ChessLogic/CGChessBoard.h"

UCGKingMovement::UCGKingMovement()
{
	Directions = { EDir::NORTH, EDir::NORTH_EAST, EDir::EAST, EDir::SOUTH_EAST, EDir::SOUTH, EDir::SOUTH_WEST, EDir::WEST, EDir::NORTH_WEST };
	Range = 1;
}

void UCGKingMovement::GetAvailableMoves(TSet<ACGTile*>& oSet)
{
	Super::GetAvailableMoves(oSet);
	ACGKing* piece = GetOwner<ACGKing>();
	if (piece && piece->Tile && piece->Board)
	{
		piece->CastleTiles.Empty();
		//can castle?
		if (!piece->IsMoved())
		{
			piece->Board->RebuildAttackMap(piece->IsWhite());
			for (EDir dir : {EDir::EAST, EDir::WEST})
			{
				int distance { 0 };
				ACGTile* tileAtTwoDistance { nullptr };
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
								oSet.Add(tileAtTwoDistance);
							}
							else
							{
								//own piece blocks the way
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