// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
#include "ChessLogic/CGChessBoard.h"
#include "MaterialShared.h"
#include "UI/CGHUD.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/CGBPUtils.h"

ACGGameMode::ACGGameMode()
{
	DefaultPawnClass = ACGChessPlayerPawn::StaticClass();
	PlayerControllerClass = ACGChessPlayerController::StaticClass();
	HUDClass = ACGHUD::StaticClass();
}

bool ACGGameMode::ReadyToStartMatch_Implementation()
{
    if (NumPlayers == 2 || UCGBPUtils::IsHotSeatMode(this) || UCGBPUtils::IsStandalone(this))
    {
        return true;
    }
    return false;
}

bool ACGGameMode::ReadyToEndMatch_Implementation()
{
    if (NumPlayers < 2 && !UCGBPUtils::IsHotSeatMode(this))
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
    if (UWorld* w = GetWorld())
    {
        ACGGameState* state = w->GetGameState<ACGGameState>();
        ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>();
        ACGChessBoard* board = UCGBPUtils::FindBoard(this);
        if (state && mode && board)
        {
            FString fen = UGameplayStatics::ParseOption(mode->OptionsString, "Fen");
            if (UCGBPUtils::IsHotSeatMode(this) || UCGBPUtils::IsStandalone(this))//spawn pieces for the menu?
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
    Super::HandleMatchHasStarted();
}

