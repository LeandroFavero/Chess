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
		
	}
	if (InGameTemplate && !InGame)
	{
		
	}
}

ACGHUD::ACGHUD()
{
	
}

void ACGHUD::Init()
{
	if (MainMenuTemplate) 
	{
		MainMenu = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenuTemplate);
	}
	if (InGameTemplate)
	{
		InGame = CreateWidget<UUserWidget>(GetOwningPlayerController(), InGameTemplate);
	}
	CurrentWidget = nullptr;
}

void ACGHUD::ShowMenu()
{
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

