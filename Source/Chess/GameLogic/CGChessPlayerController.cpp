// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessPlayerController.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGChessBoard.h"
#include "Net/UnrealNetwork.h"
#include "GameLogic/CGGameState.h"
#include "UI/CGHUD.h"
#include "GameLogic/CGGameInstance.h"

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

void ACGChessPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ACGChessPlayerController, SelectedSkinId)

	DOREPLIFETIME(ACGChessPlayerController, PreferredSide)
}

void ACGChessPlayerController::BeginPlayingState()
{
	if (UCGGameInstance* gi = GetGameInstance<UCGGameInstance>())
	{
		gi->LoadCfg();

	}
	if (ACGHUD* hud = GetHUD<ACGHUD>())
	{
		hud->ShowHud();
	}
}