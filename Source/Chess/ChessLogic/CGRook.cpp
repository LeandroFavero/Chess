// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGRook.h"
#include "CGLinearMovement.h"
#include "ChessLogic/CGTile.h"

ACGRook::ACGRook()
{
	UCGLinearMovement* moveComp = CreateDefaultSubobject<UCGLinearMovement>(TEXT("MoveValidator"));
	moveComp->Directions = { EDir::NORTH, EDir::EAST, EDir::SOUTH, EDir::WEST };
	AddOwnedComponent(moveComp);

	//Flags |= 0x00000001;//Captured order
	Flags |= EPieceFlags::CanCastle;
}
