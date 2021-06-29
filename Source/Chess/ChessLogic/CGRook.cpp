// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGRook.h"
#include "CGDirectionalMovement.h"

ACGRook::ACGRook()
{
	UCGPieceMovementBase* moveComp = CreateDefaultSubobject<UCGDirectionalMovement>(TEXT("RookMovement"));
	moveComp->Range = -1;
	AddOwnedComponent(moveComp);
}
