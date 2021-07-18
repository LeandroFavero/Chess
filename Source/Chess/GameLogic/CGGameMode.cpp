// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "MaterialShared.h"
#include "UI/CGHUD.h"
#include "ChessLogic/CGPiece.h"
#include "Kismet/GameplayStatics.h"
#include "GameLogic/CGGameState.h"

ACGGameMode::ACGGameMode()
{
	DefaultPawnClass = ACGChessPlayerPawn::StaticClass();
	PlayerControllerClass = ACGChessPlayerController::StaticClass();
	HUDClass = ACGHUD::StaticClass();
}

void ACGGameMode::BeginPlay()
{
    FString m = UGameplayStatics::ParseOption(OptionsString, "Mode");
    if (m.Equals("HS", ESearchCase::IgnoreCase))
    {
        //solo mode
        bHotSeatMode = true;
        if (ACGGameState* state = GetGameState<ACGGameState>())
        {
            if (state->Board)
            {
                FString fen = UGameplayStatics::ParseOption(OptionsString, "Fen");
                state->Board->StartGame(fen, GetWorld()->GetFirstPlayerController<ACGChessPlayerController>());
            }
        }
    }
}

void ACGGameMode::GenericPlayerInitialization(AController* Controller)
{
    if (GetNumPlayers() == 2)
    {

    }
    /*if (UWorld* w = GetWorld())
    {
        if (w->GetNumPlayerControllers() == 2 )
        {
            
        }
    }*/
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