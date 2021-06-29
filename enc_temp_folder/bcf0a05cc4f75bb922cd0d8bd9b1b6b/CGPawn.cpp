// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGChessBoard.h"

ACGPawn::ACGPawn()
{
	
}

bool ACGPawn::IsDoubleOpenAvailable()
{
	if (Board)
	{
		if (IsBlack)
		{
			return Position.Y == Board->Size.Y - 1;
		}
		else
		{
			return Position.Y == 1;
		}
	}
	return false;
}