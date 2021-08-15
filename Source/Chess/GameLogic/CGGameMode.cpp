// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "ChessLogic/CGChessBoard.h"
#include "MaterialShared.h"
#include "UI/CGHUD.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/CGBPUtils.h"
#include "UCI/UCIEngineHandler.h"

ACGGameMode::ACGGameMode()
{
	DefaultPawnClass = ACGChessPlayerPawn::StaticClass();
	PlayerControllerClass = ACGChessPlayerController::StaticClass();
	HUDClass = ACGHUD::StaticClass();
}

bool ACGGameMode::ReadyToStartMatch_Implementation()
{
	if (NumPlayers == 2 || UCGBPUtils::IsStandalone(this))
	{
		if (UCGBPUtils::IsChessEngineMode(this))
		{
			if (!EngineHandler)
			{
				if (UWorld* w = GetWorld())
				{
					ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>();
					ACGChessBoard* board = UCGBPUtils::FindBoard(this);
					ACGChessPlayerController* pc = w->GetFirstPlayerController<ACGChessPlayerController>();
					if (mode && board && pc)
					{
						EngineHandler = NewObject< UUCIEngineHandler>(this);
						EngineHandler->StartEngine(
							UGameplayStatics::ParseOption(mode->OptionsString, "Path"),
							FCString::Atoi(*UGameplayStatics::ParseOption(mode->OptionsString, "Time")),
							FCString::Atoi(*UGameplayStatics::ParseOption(mode->OptionsString, "Elo")),
							w->GetFirstPlayerController<ACGChessPlayerController>(),
							board);
						pc->OnMove.AddDynamic(this, &ACGGameMode::CheckIfEngineShouldStartThinking);
					}
				}
			}
			else if (EngineHandler->IsReady())
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool ACGGameMode::ReadyToEndMatch_Implementation()
{
	if (NumPlayers < 2 && !UCGBPUtils::IsStandalone(this))
	{
		if (ACGGameState* state = GetWorld()->GetGameState<ACGGameState>())
		{
			state->GameResult = EGameResult::DISCONNECT;
			if (UCGBPUtils::IsLocalUpdateRequired(this))
			{
				state->ResultNotify();
			}
		}
		return true;
	}
	return false;
}

void ACGGameMode::HandleMatchHasStarted()
{
	if (UWorld* w = GetWorld())
	{
		ACGGameState* state = w->GetGameState<ACGGameState>();
		ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>();
		ACGChessBoard* board = UCGBPUtils::FindBoard(this);
		if (state && mode && board)
		{
			FString fen = UGameplayStatics::ParseOption(mode->OptionsString, "Fen");
			if (UCGBPUtils::IsStandalone(this))//spawn pieces for the menu?
			{
				board->StartGame(fen, w->GetFirstPlayerController<ACGChessPlayerController>());
				if (UCGBPUtils::IsChessEngineMode(this))
				{
					CheckIfEngineShouldStartThinking();
				}
			}
			else
			{
				auto it = w->GetPlayerControllerIterator();
				ACGChessPlayerController* pc1 = Cast<ACGChessPlayerController>((*it).Get());
				++it;
				ACGChessPlayerController* pc2 = Cast<ACGChessPlayerController>((*it).Get());
				board->StartGame(fen, pc1, pc2);
			}
		}
	}
	Super::HandleMatchHasStarted();
}

void ACGGameMode::CheckIfEngineShouldStartThinking()
{
	if (UCGBPUtils::IsChessEngineMode(this))
	{
		if (UWorld* w = GetWorld())
		{
			ACGChessBoard* board = UCGBPUtils::FindBoard(this);
			ACGChessPlayerController* pc = w->GetFirstPlayerController<ACGChessPlayerController>();
			if (pc && board && pc->bIsBlack != board->IsNextMoveBlack() && EngineHandler && EngineHandler->IsReady())
			{
				EngineHandler->GetNextMove();
			}
		}
	}
}
