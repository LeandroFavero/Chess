
#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGGameState.h"
#include "GameLogic/CGGameInstance.h"
#include "GameLogic/CGSettingsSave.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGChessBoard.h"
#include "UI/CGHUD.h"
#include "UCI/UCIEngineHandler.h"
#include "Blueprint/CGBPUtils.h"
#include "MaterialShared.h"
#include "Kismet/GameplayStatics.h"

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
					UCGGameInstance* insta = w->GetGameInstance<UCGGameInstance>();
					ACGChessBoard* board = UCGBPUtils::FindBoard(this);
					ACGChessPlayerController* pc = w->GetFirstPlayerController<ACGChessPlayerController>();
					if (mode && board && pc && insta)
					{
						UCGSettingsSave* settings = insta->Settings;
						int elo = FCString::Atoi(*UGameplayStatics::ParseOption(mode->OptionsString, "Elo"));
						int time = FCString::Atoi(*UGameplayStatics::ParseOption(mode->OptionsString, "Time"));
						FString path = UGameplayStatics::ParseOption(mode->OptionsString, "Path");
						if (settings && (elo != settings->EngineElo || time != settings->EngineThinkTime || path != settings->EnginePath))
						{
							settings->EngineElo = elo;
							settings->EngineThinkTime = time;
							settings->EnginePath = path;
							insta->SaveCfg();
						}
						EngineHandler = NewObject<UUCIEngineHandler>(this);
						if (!EngineHandler->StartEngine(
							path,
							time,
							elo,
							w->GetFirstPlayerController<ACGChessPlayerController>(),
							board))
						{
							return true;
						}
						pc->OnMove.AddDynamic(EngineHandler, &UUCIEngineHandler::CheckIfEngineShouldStartThinking);
						pc->OnStart.AddDynamic(EngineHandler, &UUCIEngineHandler::CheckIfEngineShouldStartThinking);
					}
				}
			}
			else if (EngineHandler->IsReady() ||EngineHandler->IsError())
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
	Super::HandleMatchHasStarted();
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
}
