// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGKing.h"
#include "CGKingMovement.h"
#include "GameLogic/CGBoardTile.h"

ACGKing::ACGKing()
{
	UCGKingMovement* moveComp = CreateDefaultSubobject<UCGKingMovement>(TEXT("MoveValidator"));
	//moveComp->Directions = { EDir::NORTH, EDir::NORTH_EAST, EDir::EAST, EDir::SOUTH_EAST, EDir::SOUTH, EDir::SOUTH_WEST, EDir::WEST, EDir::NORTH_WEST };
	//moveComp->Range = 1;
	AddOwnedComponent(moveComp);

	//Flags |= 0x00000011;//Captured order
	Flags |= EPieceFlags::CanCastle;
}

bool ACGKing::IsInCheck()
{
	if (Tile)
	{
		return Tile->AttackedBy.Num() > 0;
	}
	return false;
}
