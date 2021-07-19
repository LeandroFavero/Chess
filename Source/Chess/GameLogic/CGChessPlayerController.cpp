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
	if (ACGGameState* state = GetWorld()->GetGameState<ACGGameState>())
	{
		if (state->IsMatchInProgress())
		{
			if (ACGChessBoard* board = UCGBPUtils::FindBoard(this))
			{
				board->UndoTo(pMoveNum);
			}
		}
	}
}

void ACGChessPlayerController::ServerConcede_Implementation()
{
	ACGGameMode* mode = GetWorld()->GetAuthGameMode<ACGGameMode>();
	ACGGameState* state = GetWorld()->GetGameState<ACGGameState>();
	ACGChessBoard* board = UCGBPUtils::FindBoard(this);
	if (mode && state && board)
	{
		if (UCGBPUtils::IsHotSeatMode(this))
		{
			if (board->Undos.Num() == 0)
			{
				state->GameResult = EGameResult::BLACK_WINS;
				mode->EndMatch();
			}
			else
			{
				state->GameResult = board->Undos.Last().LastMoveIsBlack ? EGameResult::BLACK_WINS : EGameResult::WHITE_WINS;
				mode->EndMatch();
			}
			state->ResultNotify();
		}
		else
		{
			state->GameResult = bIsBlack ? EGameResult::WHITE_WINS : EGameResult::BLACK_WINS;
			mode->EndMatch();

			if (UCGBPUtils::IsLocalUpdateRequired(this)) 
			{
				state->ResultNotify();
			}
		}
	}
}

void ACGChessPlayerController::ServerDisconnect_Implementation()
{
	if (UCGBPUtils::IsHotSeatMode(this))
	{
		//just navigate to the main menu
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Game")));
	}
	else
	{
		//UCGBPUtils::IsList
		UCGGameInstance* insta = GetGameInstance<UCGGameInstance>();
		insta->DestroySession();
	}
}

void ACGChessPlayerController::BackToMenu()
{
	if (UCGBPUtils::IsHotSeatMode(this))
	{
		//just navigate to the main menu
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Game")));
	}
	else
	{
		//UCGBPUtils::IsList
		UCGGameInstance* insta = GetGameInstance<UCGGameInstance>();
		insta->DestroySession();
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
	/*if (ACGHUD* hud = GetHUD<ACGHUD>())
	{
		if (ACGGameMode* mode = GetWorld()->GetAuthGameMode<ACGGameMode>())
		{
			if (mode->bHotSeatMode || UCGBPUtils::IsListenServer(this))
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
	}*/
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