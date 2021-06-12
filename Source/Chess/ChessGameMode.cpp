// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChessGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "UI/CGHUD.h"

AChessGameMode::AChessGameMode()
{
	DefaultPawnClass = ACGChessPlayerPawn::StaticClass();
	PlayerControllerClass = ACGChessPlayerController::StaticClass();
	HUDClass = ACGHUD::StaticClass();
}