// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLogic/CGPawn.h"
#include "ChessLogic/CGChessBoard.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGPawnMovement.h"
#include "GameLogic/CGChessPlayerController.h"
#include "GameLogic/CGGameState.h"

ACGPawn::ACGPawn()
{
	UCGPieceMovementBase* moveComp = CreateDefaultSubobject<UCGPawnMovement>(TEXT("MoveValidator"));
	AddOwnedComponent(moveComp);
}

void ACGPawn::MoveToTileInternal(ACGTile* pTile, FCGUndo& undo, bool pEvents)
{
	//en passant capture
	if (EnPassantTile && EnPassantTile == pTile)
	{
		ACGTile* otherPawnTile = EnPassantTile->Neighbours[(IsBlack() ? EDir::NORTH : EDir::SOUTH)];
		if (ACGPawn* otherPawn = Cast<ACGPawn>(otherPawnTile->OccupiedBy))
		{
			otherPawn->Flags |= EPieceFlags::EnPassantCaptured;
			undo.Capture = otherPawn;
			otherPawn->Capture(pEvents);
		}
	}
	//en passant restore
	if ((Flags & EPieceFlags::EnPassantCaptured) == EPieceFlags::EnPassantCaptured)
	{
		//fix the position of this pawn
		Super::MoveToTileInternal(pTile->Neighbours[IsBlack() ? EDir::SOUTH : EDir::NORTH], undo, pEvents);
		Flags = Flags & ~EPieceFlags::EnPassantCaptured;
	}
	else
	{
		Super::MoveToTileInternal(pTile, undo, pEvents);
	}
	//promotion
	if (pTile->Position.Y == (IsBlack() ? 0 : Board->Size.Y - 1) && pEvents)
	{
		Capture();
		undo.Promotion = this;
		//notify controller
		if (ACGChessPlayerController* pc = GetCGController())
		{
			pc->ClientBeginPromotion();
		}
	}
}

void ACGPawn::FinishPromotion(const FString& chr, FCGUndo& undo)
{
	if (ACGGameState* gameState = GetWorld()->GetGameState<ACGGameState>())
	{
		FCGUndo dummyUndo;//we don't want to undo the initial piece spawns!
		TSubclassOf<class ACGPiece>* temp = gameState->PieceTemplates.FindByPredicate([&](const TSubclassOf<class ACGPiece>& t) {
			if (const ACGPiece* p = t.Get()->GetDefaultObject<ACGPiece>())
			{
				return (p->GetFenChars().Contains(chr) && p->IsValidForPromotion());
			}
			return false;
		});
		if (temp)
		{
			//spawn new piece
			FActorSpawnParameters params;
			params.Owner = this;
			ACGPiece* newPiece = GetWorld()->SpawnActor<ACGPiece>(*temp, params);
			//newPiece->SetMaterial(isWhite ? gameState->WhiteMaterial : gameState->BlackMaterial);
			newPiece->SetColor(undo.Piece->IsWhite());
			newPiece->Board = Board;
			newPiece->MoveToTileInternal(undo.To, dummyUndo, false);

			undo.Promotion = newPiece;
			newPiece->ClientSnapToPlace();

			Board->Pieces.Add(newPiece);
			//TODO: update the notation
		}
	}
}
