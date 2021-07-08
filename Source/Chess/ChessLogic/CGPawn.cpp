// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGPawnMovement.h"

ACGPawn::ACGPawn()
{
	UCGPieceMovementBase* moveComp = CreateDefaultSubobject<UCGPawnMovement>(TEXT("MoveValidator"));
	AddOwnedComponent(moveComp);

	//Flags |= 0x00000100;//Captured order
}

/*bool ACGPawn::IsDoubleOpenAvailable()
{
	if (Board)
	{
		if (IsBlack())
		{
			return Position.Y == Board->Size.Y - 2;
		}
		else
		{
			return Position.Y == 1;
		}
	}
	return false;
}*/

void ACGPawn::BeginPromotion()
{

}

void ACGPawn::EndPromotion()
{

}