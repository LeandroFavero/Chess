// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/CGGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameLogic/CGChessPlayerController.h"

void ACGGameState::ClientGameFinished_Implementation(const EGameResult pResult)
{
	if (UWorld* w = GetWorld())
	{
		if (ACGChessPlayerController* pc = Cast<ACGChessPlayerController>(w->GetFirstPlayerController()))
		{
			switch (pResult)
			{
			case EGameResult::DRAW:
				pc->OnDraw();
				break;

			case EGameResult::WHITE_WINS:
				pc->IsBlack ? pc->OnLose() : pc->OnWin();
				break;
				
			case EGameResult::BLACK_WINS:
				pc->IsBlack ? pc->OnWin() : pc->OnLose();
				break;
			}
		}
	}
}

void ACGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGGameState, Board)
}
