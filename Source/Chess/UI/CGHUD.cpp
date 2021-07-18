// Fill out your copyright notice in the Description page of Project Settings.

#include "CGHUD.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGTile.h"
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
	if (InGameTemplate)
	{
		InGame = CreateWidget<UUserWidget>(GetOwningPlayerController(), InGameTemplate);
	}

}

FString ACGHUD::UndoToNotationString(const FCGUndo& undo) const
{
	if (undo.Imported)
	{
		return FString(TEXT("Imported board state."));
	}
	if (!undo.Piece || !undo.From || !undo.To || !undo.Piece->Board)
	{
		return FString(TEXT("ERROR"));
	}
	
	//special cases
	//castling
	if (undo.CastleRookTile)
	{
		return FString::Printf(TEXT("%d. %s"), undo.MoveNumber + 1, undo.CastleRookTile->Position.X < undo.Piece->Position.X ? "0-0" : "0-0-0");
	}
	//{number}. {piece}{fromX if not simple}{fromY if not simple}{'x' if capture '-' if simple otherwise nothing}{toX}{toY}{piece chosen from promotion (if promotion)}{+ if check # if checkmate}
	FString ret;
	ret.AppendInt(undo.MoveNumber + 1);
	ret.Append(". ");
	ret.Append(*undo.Piece->GetUnicode());
	if (!undo.SimpleNotation)
	{
		ret.AppendChar('A' + (undo.Piece->Board->Size.X - undo.From->Position.X));
		ret.AppendChar('1' + undo.From->Position.Y);
	}
	if (undo.Capture || !undo.SimpleNotation)
	{
		ret.AppendChar(undo.Capture ? 'x' : '-');
	}
	ret.AppendChar('A' + (undo.Piece->Board->Size.X - undo.To->Position.X));
	ret.AppendChar('1' + undo.To->Position.Y);
	if (undo.Promotion)
	{
		if (undo.Promotion->GetFenChars().Len() > 0)
		{
			ret.AppendChar(undo.Promotion->GetFenChars().ToUpper()[0]);
		}
	}
	if (undo.Check)
	{
		ret.AppendChar('+');
		//TODO: # if checkhogymate
	}
	return ret;
	/*return FString::Printf(
			TEXT("%d. %s%s%s%s%c%c"),
			undo.MoveNumber + 1,
			*undo.Piece->GetUnicode(),
			'A' + (undo.Piece->Board->Size.X - undo.From->Position.X),
			'1' + undo.From->Position.Y,
			undo.Capture ? 'x' : '-',
			'A' + (undo.Piece->Board->Size.X - undo.To->Position.X),
			'1' + undo.To->Position.Y);*/
};