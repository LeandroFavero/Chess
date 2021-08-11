// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGBishop.h"
#include "ChessLogic/CGLinearMovement.h"

const FString ACGBishop::BishopFen = { TEXT("Bb") };

ACGBishop::ACGBishop()
{
	UCGLinearMovement* moveComp = CreateDefaultSubobject<UCGLinearMovement>(TEXT("MoveValidator"));
	moveComp->Directions = { EDir::NORTH_EAST, EDir::SOUTH_EAST, EDir::SOUTH_WEST, EDir::NORTH_WEST };
	AddOwnedComponent(moveComp);
}

const bool ACGBishop::IsFenMatches(const TCHAR& iChr) const
{
	int idx;
	return BishopFen.FindChar(iChr, idx);
}

const FString ACGBishop::GetFenChar() const
{
	return IsWhite() ? TEXT("B") : TEXT("b");
}