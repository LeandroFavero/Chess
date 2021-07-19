// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/CGGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameLogic/CGChessPlayerController.h"
#include "ChessLogic/CGChessBoard.h"
#include "UI/CGHUD.h"
#include "Blueprint/CGBPUtils.h"


void ACGGameState::ColorsChanged()
{
	if (ACGChessBoard* board = UCGBPUtils::FindBoard(this))
	{
		board->RefreshPieceColors();
	}
}

void ACGGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	//show in game or menu
	if (ACGChessPlayerController* pc = GetWorld()->GetFirstPlayerController<ACGChessPlayerController>())
	{
		if (ACGHUD* hud = pc->GetHUD<ACGHUD>())
		{
			if (UCGBPUtils::IsHotSeatMode(this) || UCGBPUtils::IsListenServer(this) || UCGBPUtils::IsClient(this))
			{
				hud->ShowGame();
			}
			else
			{
				hud->ShowMenu();
			}
		}
		pc->OnStart.Broadcast();
	}
}

void ACGGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	if (UWorld* w = GetWorld())
	{
		if (ACGChessPlayerController* pc = Cast<ACGChessPlayerController>(w->GetFirstPlayerController()))
		{
			pc->OnGameOver.Broadcast(GameResult);
			/*switch (GameResult)
			{
			case EGameResult::DRAW:
				pc->OnDraw();
				break;

			case EGameResult::WHITE_WINS:
				pc->bIsBlack ? pc->OnLose() : pc->OnWin();
				break;

			case EGameResult::BLACK_WINS:
				pc->bIsBlack ? pc->OnWin() : pc->OnLose();
				break;
			}*/
		}
	}
}

void ACGGameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		if (ACGHUD* hud = pc->GetHUD<ACGHUD>())
		{
			hud->ShowGame();
		}
	}
}

bool ACGGameState::IsMatchInProgress() const
{
	return Super::IsMatchInProgress();
}

void ACGGameState::ResultNotify()
{
	
}

void ACGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGGameState, BlackMaterial)
	DOREPLIFETIME(ACGGameState, WhiteMaterial)
}


