// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGQueen.h"
#include "CGLinearMovement.h"

const FString ACGQueen::QueenFen = { TEXT("Qq") };

ACGQueen::ACGQueen()
{
	UCGLinearMovement* moveComp = CreateDefaultSubobject<UCGLinearMovement>(TEXT("MoveValidator"));
	moveComp->Directions = { EDir::NORTH, EDir::NORTH_EAST, EDir::EAST, EDir::SOUTH_EAST, EDir::SOUTH, EDir::SOUTH_WEST, EDir::WEST, EDir::NORTH_WEST };
	AddOwnedComponent(moveComp);
}

const bool ACGQueen::IsFenMatches(const TCHAR& iChr) const
{
	int idx;
	return QueenFen.FindChar(iChr, idx);
}

const FString ACGQueen::GetFenChar() const
{
	return IsWhite() ? TEXT("Q") : TEXT("q");
}