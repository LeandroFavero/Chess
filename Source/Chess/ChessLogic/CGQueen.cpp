// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLogic/CGQueen.h"
#include "CGLinearMovement.h"
#include "ChessLogic/CGTile.h"

ACGQueen::ACGQueen()
{
	UCGLinearMovement* moveComp = CreateDefaultSubobject<UCGLinearMovement>(TEXT("MoveValidator"));
	moveComp->Directions = { EDir::NORTH, EDir::NORTH_EAST, EDir::EAST, EDir::SOUTH_EAST, EDir::SOUTH, EDir::SOUTH_WEST, EDir::WEST, EDir::NORTH_WEST };
	AddOwnedComponent(moveComp);
}

