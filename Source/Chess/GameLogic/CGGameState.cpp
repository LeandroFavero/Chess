// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/CGGameState.h"
#include "Net/UnrealNetwork.h"

void ACGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGGameState, Board)
	//DOREPLIFETIME(ACGGameState, OnValidMove)
	//DOREPLIFETIME(ACGGameState, OnRollbackTo)
}
