// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawnMovement.h"
#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGTile.h"
#include "GameLogic/CGUndo.h"
#include "CGChessBoard.h"

//TODO: promotion !!!

void UCGPawnMovement::AvailableMoves(TSet<ACGTile*>& set)
{
	ACGPawn* pawn = GetOwner<ACGPawn>();
	if (pawn && pawn->Tile && pawn->Board)
	{
		ACGTile* t = pawn->Tile->Neighbours[pawn->IsBlack() ? EDir::SOUTH : EDir::NORTH];
		if (t)
		{
			if (t->OccupiedBy.Num() == 0)
			{
				set.Add(t);
				//can double open?
				if ((pawn->IsBlack() && pawn->Position.Y == pawn->Board->Size.Y - 2) || (pawn->IsWhite() && pawn->Position.Y == 1))
				{
					t = t->Neighbours[pawn->IsBlack() ? EDir::SOUTH : EDir::NORTH];
					if (t && t->OccupiedBy.Num() == 0)
					{
						set.Add(t);
					}
				}
			}
		}
		//attack
		t = pawn->Tile->Neighbours[pawn->IsBlack() ? EDir::SOUTH_EAST : EDir::NORTH_EAST];
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
		t = pawn->Tile->Neighbours[pawn->IsBlack() ? EDir::SOUTH_WEST : EDir::NORTH_WEST];
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
		//en passant
		if (pawn->Board->Undos.Num() > 0 && (pawn->IsBlack() ? pawn->Position.Y == 3 : pawn->Board->Size.Y - 4))//is in the good rank
		{
			for (EDir dir : {EDir::EAST, EDir::WEST})
			{
				t = pawn->Tile->Neighbours[dir];
				if (t)
				{
					for (const ACGPiece* other : t->OccupiedBy)
					{
						const FCGUndo& undo = pawn->Board->Undos.Last();
						if (other->IsA(ACGPawn::StaticClass()) && undo.Piece == other)//are we standing next to the last moved pawn
						{
							if (undo.From && undo.To && (abs(undo.From->Position.Y - undo.To->Position.Y) == 2))//was the last move double open?
							{
								pawn->EnPassantTile = t->Neighbours[pawn->IsBlack() ? EDir::SOUTH : EDir::NORTH];
								set.Add(pawn->EnPassantTile);
								break;
							}
						}
					}
				}
			}
		}
	}
}

void UCGPawnMovement::AttackedTiles(TSet<ACGTile*>& set)
{
	ACGPawn* owner = GetOwner<ACGPawn>();
	if (owner)
	{
		ACGTile* t;
		t = owner->Tile->Neighbours[owner->IsBlack() ? EDir::SOUTH_WEST : EDir::NORTH_WEST];
		if (t)
		{
			set.Add(t);
		}
		t = owner->Tile->Neighbours[owner->IsBlack() ? EDir::SOUTH_EAST : EDir::NORTH_EAST];
		if (t)
		{
			set.Add(t);
		}
	}
	//en passant can't attack kings so it should not matter.
}