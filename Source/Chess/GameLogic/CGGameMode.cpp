// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGChessPlayerController.h"
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

/*void ACGGameMode::BeginPlay()
{
    
}*/

void ACGGameMode::StartMatch()
{
    //if hot seat or menu start the game
    if (UCGBPUtils::IsHotSeatMode(this) || UCGBPUtils::IsStandalone(this))
    {
        SetMatchState(MatchState::InProgress);
    }
    //can we not call super?
}

void ACGGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    //if we have enough players start the match
    if (NumPlayers == 2)
    {
        SetMatchState(MatchState::InProgress);
    }
}

void ACGGameMode::GenericPlayerInitialization(AController* Controller)
{
    if (GetNumPlayers() == 2)
    {
        if (UWorld* w = GetWorld())
        {
            ACGGameState* state = w->GetGameState<ACGGameState>();
            ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>();
            ACGChessBoard* board = UCGBPUtils::FindBoard(this);
            if (state && mode && board)
            {
                FString fen = UGameplayStatics::ParseOption(mode->OptionsString, "Fen");
                auto it = w->GetPlayerControllerIterator();
                ACGChessPlayerController* pc1 = Cast<ACGChessPlayerController>((*it).Get());
                ++it;
                ACGChessPlayerController* pc2 = Cast<ACGChessPlayerController>((*it).Get());
                board->StartGame(fen, pc1, pc2);
            }
        }
    }
    Super::GenericPlayerInitialization(Controller);
}

