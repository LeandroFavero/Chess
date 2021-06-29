// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChessGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "MaterialShared.h"
#include "UI/CGHUD.h"

AChessGameMode::AChessGameMode()
{
	DefaultPawnClass = ACGChessPlayerPawn::StaticClass();
	PlayerControllerClass = ACGChessPlayerController::StaticClass();
	HUDClass = ACGHUD::StaticClass();


}

int AChessGameMode::GetCurrentViewMode(const APlayerController* PlayerController)
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

/*FCGSkinData AChessGameMode::GetSkinData(FName Id)
{
	
	if (row)
	{
		return row->WhiteMaterial;
	}
	return nullptr;
}*/