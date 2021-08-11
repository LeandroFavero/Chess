// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGRook.h"
#include "CGLinearMovement.h"

const FString ACGRook::RookFen = { TEXT("Rr") };

ACGRook::ACGRook()
{
	UCGLinearMovement* moveComp = CreateDefaultSubobject<UCGLinearMovement>(TEXT("MoveValidator"));
	moveComp->Directions = { EDir::NORTH, EDir::EAST, EDir::SOUTH, EDir::WEST };
	AddOwnedComponent(moveComp);
}

const bool ACGRook::IsFenMatches(const TCHAR& iChr) const
{
	int idx;
	return RookFen.FindChar(iChr, idx);
}

const FString ACGRook::GetFenChar() const
{
	return IsWhite() ? TEXT("R") : TEXT("r");
}