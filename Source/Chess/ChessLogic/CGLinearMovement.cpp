// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGLinearMovement.h"
#include "ChessLogic/CGPiece.h"


void UCGLinearMovement::AvailableMoves(TSet<ACGBoardTile*>& set)
{
	ACGPiece* piece = GetOwner<ACGPiece>();
	if (piece)
	{
		for (EDir i : Directions)
		{
			int dir = static_cast<int>(i);
			ACGBoardTile* t = piece->Tile;
			for(int remaining = Range;t && remaining != 0; --remaining)
			{
				t = t->Neighbours[dir];
				if (t)
				{
					for (ACGPiece* other : t->OccupiedBy)
					{
						if (piece->IsBlack() != other->IsBlack())
						{
							set.Add(t);
							goto continue_directions;
						}
						else
						{
							goto continue_directions;
						}
					}
					set.Add(t);
				}
			}
		continue_directions:;
		}

	}
}