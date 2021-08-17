// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogic/CGGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameLogic/CGChessPlayerController.h"
#include "ChessLogic/CGChessBoard.h"
#include "UI/CGHUD.h"
#include "Blueprint/CGBPUtils.h"
#include "Engine/DataTable.h"
#include "GameLogic/CGSkinData.h"
#include "GameLogic/CGGameInstance.h"
#include "GameLogic/CGSettingsSave.h"

ACGGameState::ACGGameState()
{
}

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
			if (UCGBPUtils::IsHotSeatMode(this) || UCGBPUtils::IsListenServer(this) || UCGBPUtils::IsClient(this) || UCGBPUtils::IsChessEngineMode(this))
			{
				hud->ShowGame();
			}
			else
			{
				hud->ShowMenu();
			}
		}
		pc->OnStart.Broadcast();
		if (UCGGameInstance* insta = GetWorld()->GetGameInstance<UCGGameInstance>())
		{
			if (insta->Settings)
			{
				pc->ServerChangeSkin(insta->Settings->SelectedSkin);
			}
		}
	}
}

void ACGGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void ACGGameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		if (ACGHUD* hud = pc->GetHUD<ACGHUD>())
		{
			hud->Init();
			hud->ShowGame();
		}
	}
}

bool ACGGameState::IsMatchInProgress() const
{
	return Super::IsMatchInProgress();
}

void ACGGameState::EndGameBecauseOfDisconnect()
{
	SetMatchState(TEXT("WaitingPostMatch"));
}

void ACGGameState::ResultNotify()
{
	if (UWorld* w = GetWorld())
	{
		if (ACGChessPlayerController* pc = Cast<ACGChessPlayerController>(w->GetFirstPlayerController()))
		{
			pc->OnGameOver.Broadcast(static_cast<EGameResult>(GameResult));
		}
	}
}

void ACGGameState::UseSkin(const FString& iName, bool iIsBlack)
{
	if (Skins)
	{
		for (const auto& it : Skins->GetRowMap())
		{
			if (const FCGSkinData* row = (FCGSkinData*)(it.Value))
			{
				if (iName.Equals(row->Name.ToString(), ESearchCase::IgnoreCase))
				{
					if (iIsBlack || UCGBPUtils::IsStandalone(this))
					{
						BlackMaterial = row->BlackMaterial;
					}
					if(!iIsBlack || UCGBPUtils::IsStandalone(this))
					{
						WhiteMaterial = row->WhiteMaterial;
					}
					if (UCGBPUtils::IsLocalUpdateRequired(this))
					{
						ColorsChanged();
					}
					return;
				}
			}
		}

		if (iIsBlack || UCGBPUtils::IsStandalone(this))
		{
			BlackMaterial = DefaultBlack;
		}
		if(!iIsBlack || UCGBPUtils::IsStandalone(this))
		{
			WhiteMaterial = DefaultWhite;
		}
	}
}

void ACGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGGameState, BlackMaterial)
	DOREPLIFETIME(ACGGameState, WhiteMaterial)
	DOREPLIFETIME(ACGGameState, GameResult)
}


