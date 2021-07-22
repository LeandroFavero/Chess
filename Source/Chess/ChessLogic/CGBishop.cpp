// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGBishop.h"
#include "ChessLogic/CGLinearMovement.h"

ACGBishop::ACGBishop()
{
	UCGLinearMovement* moveComp = CreateDefaultSubobject<UCGLinearMovement>(TEXT("MoveValidator"));
	moveComp->Directions = { EDir::NORTH_EAST, EDir::SOUTH_EAST, EDir::SOUTH_WEST, EDir::NORTH_WEST };
	AddOwnedComponent(moveComp);
}