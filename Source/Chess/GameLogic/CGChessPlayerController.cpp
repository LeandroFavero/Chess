// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessPlayerController.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGChessBoard.h"
#include "Net/UnrealNetwork.h"
#include "GameLogic/CGGameState.h"
#include "UI/CGHUD.h"
#include "GameLogic/CGGameInstance.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGGameMode.h"
#include "Blueprint/CGBPUtils.h"

ACGChessPlayerController::ACGChessPlayerController() 
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
}

void ACGChessPlayerController::ServerMoveToTile_Implementation(ACGPiece* pPiece, ACGTile* pTile)
{
	if (pPiece && pTile)
	{
		pPiece->MoveToTile(pTile);
	}
}

void ACGChessPlayerController::ServerUpdateGrab_Implementation(ACGPiece* pPiece, FVector_NetQuantize pLocation)
{
	if (pPiece)
	{
		pPiece->SetActorLocation(FVector(pLocation.X, pLocation.Y, 0), true);
	}
}

void ACGChessPlayerController::ServerGrab_Implementation(ACGPiece* pPiece, bool pGrab)
{
	if (pPiece)
	{
		pPiece->ServerGrab(pGrab);
	}
}

void ACGChessPlayerController::ServerUndoTo_Implementation(int pMoveNum)
{
	if (UWorld* w = GetWorld())
	{
		if (ACGGameState* state = w->GetGameState<ACGGameState>())
		{
			state->Board->UndoTo(pMoveNum);
		}
	}
}

void ACGChessPlayerController::ServerConcede_Implementation()
{
	if (ACGGameState* state = GetWorld()->GetGameState<ACGGameState>())
	{
		if (UCGBPUtils::IsHotSeatMode(this))
		{
			if (state->Board->Undos.Num() == 0)
			{
				state->ClientGameFinished(EGameResult::BLACK_WINS);
			}
			else
			{
				state->ClientGameFinished(state->Board->Undos.Last().LastMoveIsBlack ? EGameResult::BLACK_WINS : EGameResult::WHITE_WINS);
			}
		}
		else
		{
			state->ClientGameFinished(bIsBlack ? EGameResult::WHITE_WINS : EGameResult::BLACK_WINS);
		}
	}
}

void ACGChessPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ACGChessPlayerController, SelectedSkinId)

	DOREPLIFETIME(ACGChessPlayerController, PreferredSide)
}

void ACGChessPlayerController::SideChanged()
{

}

void ACGChessPlayerController::BeginPlayingState()
{
	if (UCGGameInstance* gi = GetGameInstance<UCGGameInstance>())
	{
		gi->LoadCfg();

	}
	if (ACGHUD* hud = GetHUD<ACGHUD>())
	{
		if (ACGGameMode* mode = GetWorld()->GetAuthGameMode<ACGGameMode>())
		{
			if (mode->bHotSeatMode)
			{
				hud->ShowGame();
			}
			else
			{
				hud->ShowMenu();
			}
		}
		else
		{
			hud->ShowGame();
		}
	}
}

/*
void ACGChessPlayerController::SetPawn(APawn* InPawn)
{
	if (ACGGameMode* mode = GetWorld()->GetAuthGameMode<ACGGameMode>())
	{
		if (ACGChessPlayerPawn* p = Cast<ACGChessPlayerPawn>(InPawn))
		{
			p->bIsHotSeat = mode->bHotSeatMode;
		}
	}
	Super::SetPawn(InPawn);
}
*/