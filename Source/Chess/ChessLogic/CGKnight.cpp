// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGKnight.h"
#include "ChessLogic/CGKnightMovement.h"

ACGKnight::ACGKnight()
{
	UCGKnightMovement* moveComp = CreateDefaultSubobject<UCGKnightMovement>(TEXT("MoveValidator"));
	AddOwnedComponent(moveComp);

	//Flags |= 0x00000010;//Captured order
}

void ACGKnight::SetColor(bool isWhite)
{
	Super::SetColor(isWhite);
	//TODO: rotate mesh!
	if (isWhite)
	{
		Mesh->SetRelativeRotation(FRotator(0, WhiteMeshRotation, 0));
	}
	else
	{
		Mesh->SetRelativeRotation(FRotator(0, BlackMeshRotation, 0));
	}
}

