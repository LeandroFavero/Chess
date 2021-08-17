// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessPlayerController.h"
#include "Blueprint/CGBPUtils.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGChessBoard.h"
#include "GameLogic/CGGameInstance.h"
#include "GameLogic/CGChessPlayerPawn.h"
#include "GameLogic/CGGameMode.h"
#include "GameLogic/CGGameState.h"
#include "GameLogic/CGSettingsSave.h"
#include "UI/CGHUD.h"
#include "Net/UnrealNetwork.h"

ACGChessPlayerController::ACGChessPlayerController() 
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
}

void ACGChessPlayerController::ServerChangeSkin_Implementation(const FString& iName)
{
	if (ACGGameState* state = GetWorld()->GetGameState<ACGGameState>())
	{
		state->UseSkin(iName, bIsBlack);
	}
}

void ACGChessPlayerController::ServerMoveToTile_Implementation(ACGPiece* iPiece, ACGTile* iTile)
{
	if (iPiece && iTile && iPiece->Board)
	{
		if (iPiece->IsBlack() == bIsBlack || UCGBPUtils::IsStandalone(this))//NO HAX PLS
		{
			iPiece->MoveToTile(iTile);
			iPiece->Board->GameOverCheck();
		}
	}
}

void ACGChessPlayerController::ServerUpdateGrab_Implementation(ACGPiece* iPiece, FVector_NetQuantize pLocation)
{
	if (iPiece)
	{
		iPiece->SetActorLocation(FVector(pLocation.X, pLocation.Y, 0), true);
	}
}

void ACGChessPlayerController::ServerGrab_Implementation(ACGPiece* iPiece, bool iIsGrab)
{
	if (iPiece)
	{
		iPiece->Grab(iIsGrab);
	}
}

void ACGChessPlayerController::ServerUndoTo_Implementation(int iMoveNum)
{
	if (ACGGameState* state = GetWorld()->GetGameState<ACGGameState>())
	{
		if (state->IsMatchInProgress())
		{
			if (ACGChessBoard* board = UCGBPUtils::FindBoard(this))
			{
				board->UndoTo(iMoveNum);
			}
		}
	}
}

void ACGChessPlayerController::ServerConcede_Implementation()
{
	ACGChessBoard* board = UCGBPUtils::FindBoard(this);
	if (board)
	{
		if (UCGBPUtils::IsHotSeatMode(this))
		{
			board->EndGame(board->IsNextMoveBlack() ? EGameResult::BLACK_WINS : EGameResult::WHITE_WINS);
		}
		else
		{
			board->EndGame(bIsBlack ? EGameResult::WHITE_WINS : EGameResult::BLACK_WINS);
		}
	}
}

void ACGChessPlayerController::ServerChoosePromotion_Implementation(const FString& iPieceType)
{
	if (ACGChessBoard* board = UCGBPUtils::FindBoard(this))
	{
		if (!iPieceType.IsEmpty())
		{
			if (board->Undos.Num() > 0)
			{
				FCGUndo& u = board->Undos.Last();
				if (u.Piece == u.Promotion)
				{
					if (ACGPawn* pawn = Cast<ACGPawn>(u.Piece))
					{
						pawn->FinishPromotion(iPieceType, u);
						board->GameOverCheck();
					}
				}
			}
		}
	}
}

void ACGChessPlayerController::ServerClaimDraw_Implementation()
{
	if (bIsDrawClaimable)
	{
		if (ACGChessBoard* board = UCGBPUtils::FindBoard(this))
		{
			board->GameOverCheck(true);
		}
	}
}

void ACGChessPlayerController::ClientBeginPromotion_Implementation()
{
	OnPromotion.Broadcast();
}

void ACGChessPlayerController::BackToMenu()
{
	if (UCGBPUtils::IsHotSeatMode(this) || UCGBPUtils::IsChessEngineMode(this))
	{
		//just navigate to the main menu
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Game")));
	}
	else
	{
		UCGGameInstance* insta = GetGameInstance<UCGGameInstance>();
		insta->DestroySession();
	}
}

void ACGChessPlayerController::SideChanged()
{

}

void ACGChessPlayerController::DrawClaimableChanged()
{
	OnDrawClaimable.Broadcast(bIsDrawClaimable);
}

void ACGChessPlayerController::BeginPlay()
{
	if (IsLocalPlayerController())
	{
		if (UCGGameInstance* gi = GetGameInstance<UCGGameInstance>())
		{
			gi->LoadCfg();
			PreferredSide = gi->Settings->PreferredSide;
			//Skin
		}
	}
	Super::BeginPlay();
}

void ACGChessPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ACGChessPlayerController, PreferredSide)
	DOREPLIFETIME(ACGChessPlayerController, bIsBlack)
	DOREPLIFETIME(ACGChessPlayerController, bIsDrawClaimable)
}
