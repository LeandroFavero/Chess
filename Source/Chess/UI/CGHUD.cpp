// Fill out your copyright notice in the Description page of Project Settings.

#include "CGHUD.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGChessBoard.h"

#define Dbg(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

void ACGHUD::InitHUD()
{
	if (MainMenuTemplate && !MainMenu)
	{
		MainMenu = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenuTemplate);
	}
	if (InGameTemplate && !InGame)
	{
		InGame = CreateWidget<UUserWidget>(GetOwningPlayerController(), InGameTemplate);
	}
}

ACGHUD::ACGHUD()
{
	
}

void ACGHUD::ShowMenu()
{
	InitHUD();
	if (CurrentWidget != MainMenu)
	{
		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromViewport();
		}
		if (MainMenu)
		{
			MainMenu->AddToViewport();
			CurrentWidget = MainMenu;
		}
	}
}

void ACGHUD::ShowGame()
{
	InitHUD();
	if (CurrentWidget != InGame)
	{
		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromViewport();
		}
		if (InGame)
		{
			InGame->AddToViewport();
			CurrentWidget = InGame;
		}
	}
}

