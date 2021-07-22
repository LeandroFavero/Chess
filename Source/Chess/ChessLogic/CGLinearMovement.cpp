// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGLinearMovement.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGKing.h"


void UCGLinearMovement::GetAvailableMoves(TSet<ACGTile*>& oSet)
{
	Generate(oSet, false);
}

void UCGLinearMovement::GetAttackedTiles(TSet<ACGTile*>& oSet)
{
	Generate(oSet, true);
}

void UCGLinearMovement::Generate(TSet<ACGTile*>& oSet, bool pIsAttack)
{
	ACGPiece* piece = GetOwner<ACGPiece>();
	if (piece)
	{
		for (EDir dir : Directions)
		{
			ACGTile* t = piece->Tile;
			for (int remaining = Range; t && remaining != 0; --remaining)
			{
				t = t->Neighbours[dir];
				if (t)
				{
					if (t->OccupiedBy)
					{
						if (piece->IsBlack() != t->OccupiedBy->IsBlack())
						{
							oSet.Add(t);
							break;
						}
						else
						{
							if (pIsAttack)
							{
								oSet.Add(t);
							}
							break;
						}
					}
					oSet.Add(t);
				}
			}
		}
	}
}