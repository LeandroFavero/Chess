// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "MaterialShared.h"
#include "UI/CGHUD.h"
#include "ChessLogic/CGCanBePromotedInto.h"
#include "ChessLogic/CGPiece.h"


ACGGameMode::ACGGameMode()
{
	DefaultPawnClass = ACGChessPlayerPawn::StaticClass();
	PlayerControllerClass = ACGChessPlayerController::StaticClass();
	HUDClass = ACGHUD::StaticClass();


}

int ACGGameMode::GetCurrentViewMode(const APlayerController* PlayerController)
{

    if (IsValid(PlayerController))
    {
        UGameViewportClient* GameViewportClient = PlayerController->GetWorld()->GetGameViewport();
        ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

        bool ignore = GameViewportClient->IgnoreInput();
        EMouseCaptureMode capt = GameViewportClient->GetMouseCaptureMode();

        if (ignore == false && capt == EMouseCaptureMode::CaptureDuringMouseDown)
        {
            return 0;  // Game And UI
        }
        else if (ignore == true && capt == EMouseCaptureMode::NoCapture)
        {
            return 1;  // UI Only
        }
        else
        {
            return 2;  // Game Only
        }
    }

    return -1;

}