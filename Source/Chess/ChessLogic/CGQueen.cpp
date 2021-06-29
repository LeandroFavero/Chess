// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGQueen.h"

#include "CGDirectionalMovement.h"
#include "CGDiagonalMovement.h"

ACGQueen::ACGQueen()
{
	UCGPieceMovementBase* moveComp = CreateDefaultSubobject<UCGDirectionalMovement>(TEXT("RookMovement"));
	moveComp->Range = -1;
	AddOwnedComponent(moveComp);
	moveComp = CreateDefaultSubobject<UCGDiagonalMovement>(TEXT("BishopMovement"));
	moveComp->Range = -1;
	AddOwnedComponent(moveComp);
}

