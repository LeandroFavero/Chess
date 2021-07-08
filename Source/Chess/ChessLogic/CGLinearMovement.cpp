// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGLinearMovement.h"
#include "ChessLogic/CGPiece.h"


void UCGLinearMovement::AvailableMoves(TSet<ACGBoardTile*>& set)
{
	ACGPiece* owner = GetOwner<ACGPiece>();
	if (owner)
	{
		
		for (EDir i : Directions)
		{
			int dir = static_cast<int>(i);
			ACGBoardTile* t = owner->Tile;
			for(int remaining = Range;t && remaining != 0; --remaining)
			{
				t = t->Neighbours[dir];
				if (t)
				{
					for (ACGPiece* other : t->OccupiedBy)
					{
						if (owner->IsBlack() != other->IsBlack())
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