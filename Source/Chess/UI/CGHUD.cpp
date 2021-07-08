// Fill out your copyright notice in the Description page of Project Settings.

#include "CGHUD.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGBoardTile.h"
#include "ChessLogic/CGChessBoard.h"

#define Dbg(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

void ACGHUD::ShowHud()
{
	if (MainMenuTemplate)
	{
		MainMenu = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenuTemplate);
		if (MainMenu)
		{
			MainMenu->AddToViewport();
		}
	}
	if (OptionsMenuTemplate)
	{
		OptionsMenu = CreateWidget<UUserWidget>(GetOwningPlayerController(), OptionsMenuTemplate);
	}
	if (InGameTemplate)
	{
		InGame = CreateWidget<UUserWidget>(GetOwningPlayerController(), InGameTemplate);
	}

}

//void ACGHUD::UpdateHud()
//{
//}

void ACGHUD::ShowOptions()
{
	if (OptionsMenu)
	{
		OptionsMenu->AddToViewport();
	}
}

FString ACGHUD::UndoToNotationString(const FCGUndo& undo) const
{
	if (!undo.Piece || !undo.From || !undo.To || !undo.Piece->Board)
	{
		return FString(TEXT("ERROR"));
	}
	return FString::Printf(
		TEXT("%d. %s%c%c %c%c"),
		undo.MoveNumber + 1,
		*undo.Piece->GetUnicode(), 
		'A' + (undo.Piece->Board->Size.X - undo.From->Position.X), 
		'1' + undo.From->Position.Y,
		'A' + (undo.Piece->Board->Size.X - undo.To->Position.X),
		'1' + undo.To->Position.Y);
};