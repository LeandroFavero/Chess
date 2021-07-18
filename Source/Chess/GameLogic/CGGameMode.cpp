// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "MaterialShared.h"
#include "UI/CGHUD.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGGameState.h"
#include "Kismet/GameplayStatics.h"

ACGGameMode::ACGGameMode()
{
	DefaultPawnClass = ACGChessPlayerPawn::StaticClass();
	PlayerControllerClass = ACGChessPlayerController::StaticClass();
	HUDClass = ACGHUD::StaticClass();
}

void ACGGameMode::BeginPlay()
{
    
}

void ACGGameMode::GenericPlayerInitialization(AController* Controller)
{
    if (GetNumPlayers() == 2)
    {
        if (UWorld* w = GetWorld())
        {
            ACGGameState* state = w->GetGameState<ACGGameState>();
            ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>();
            if (state && mode && state->Board)
            {
                FString fen = UGameplayStatics::ParseOption(mode->OptionsString, "Fen");
                auto it = w->GetPlayerControllerIterator();
                ACGChessPlayerController* pc1 = Cast<ACGChessPlayerController>((*it).Get());
                ++it;
                ACGChessPlayerController* pc2 = Cast<ACGChessPlayerController>((*it).Get());
                state->Board->StartGame(fen, pc1, pc2);
            }
        }
    }
    Super::GenericPlayerInitialization(Controller);
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