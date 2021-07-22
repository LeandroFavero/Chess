// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGLinearMovement.h"
#include "ChessLogic/CGPiece.h"


void UCGLinearMovement::GetAvailableMoves(TSet<ACGTile*>& set)
{
	Generate(set, false);
}

void UCGLinearMovement::GetAttackedTiles(TSet<ACGTile*>& set)
{
	Generate(set, true);
}

void UCGLinearMovement::Generate(TSet<ACGTile*>& set, bool pIsAttack)
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
							set.Add(t);
							break;
						}
						else
						{
							if (pIsAttack)
							{
								set.Add(t);
							}
							break;
						}
					}
					set.Add(t);
				}
			}
		}
	}
}