// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/CGOnlineSession.h"
#include "GameLogic/CGChessPlayerController.h"
#include "GameLogic/CGGameState.h"


void UCGOnlineSession::HandleDisconnect(UWorld* World, UNetDriver* NetDriver)
{
	//TODO: notify user
	if (World)
	{
		if (ACGGameState* state = World->GetGameState<ACGGameState>())
		{
			if (state->GameResult == EGameResult::NOT_FINISHED)
			{
				if (ACGChessPlayerController* pc = World->GetFirstPlayerController<ACGChessPlayerController>())
				{					
					state->GameResult = EGameResult::DISCONNECT;
					state->EndGameBecauseOfDisconnect();
					pc->OnGameOver.Broadcast(state->GameResult);
				}
			}
		}
	}
}
