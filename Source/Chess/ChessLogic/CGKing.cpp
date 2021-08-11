// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGKing.h"
#include "ChessLogic/CGRook.h"
#include "CGKingMovement.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGTile.h"
#include "Blueprint/CGBPUtils.h"

const FString ACGKing::KingFen = { TEXT("Kk") };

ACGKing::ACGKing()
{
	UCGKingMovement* moveComp = CreateDefaultSubobject<UCGKingMovement>(TEXT("MoveValidator"));
	AddOwnedComponent(moveComp);
}

const bool ACGKing::IsFenMatches(const TCHAR& iChr) const
{
	int idx;
	return KingFen.FindChar(iChr, idx);
}

const FString ACGKing::GetFenChar() const
{
	return IsWhite() ? TEXT("K") : TEXT("k");
}

void ACGKing::MoveToTile(ACGTile* iTile)
{
	Super::MoveToTile(iTile);
	//update rook for local player
	if (Board)
	{
		if (const FCGUndo* last = Board->GetLastUndo())
		{
			if (last->CastleRook && UCGBPUtils::IsLocalUpdateRequired(this))
			{
				last->CastleRook->SnapToPlace();
			}
		}
	}
}

void ACGKing::MoveToTileInternal(ACGTile* iTile, FCGUndo& oUndo, bool iEvents)
{
	if (!iTile)
	{
		return;
	}
	if (CastleTiles.Contains(iTile))
	{
		EDir side = Position.X > iTile->Position.X ? EDir::WEST : EDir::EAST;
		EDir otherSide = Position.X > iTile->Position.X ? EDir::EAST : EDir::WEST;
		for (ACGTile* t = iTile->Neighbours[side]; t; t = t->Neighbours[side])
		{
			if (t && t->OccupiedBy)
			{
				if (ACGRook* r = Cast<ACGRook>(t->OccupiedBy))
				{
					oUndo.CastleRook = r;
					oUndo.CastleRookTile = t;
					FCGUndo dummyUndo;
					r->MoveToTileInternal(iTile->Neighbours[otherSide], dummyUndo, false);
					//r->ClientSnapToPlace();
					break;
				}
			}
		}
	}
	Super::MoveToTileInternal(iTile, oUndo, iEvents);
}

bool ACGKing::IsInCheck()
{
	if (Tile)
	{
		return Tile->AttackedBy.Num() > 0;
	}
	return false;
}
