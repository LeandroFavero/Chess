// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGUndo.h"
#include "CGPawn.generated.h"

class ACGTile;
/**
 * 
 */
UCLASS()
class CHESS_API ACGPawn : public ACGPiece
{
	GENERATED_BODY()
public:
	ACGPawn();

	const FString GetFenChars() const override { return "Pp"; }
	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265F") : TEXT("\u2659")); }

	ACGTile* EnPassantTile = nullptr;

	void MoveToTileInternal(ACGTile* pTile, FCGUndo& undo, bool pEvents) override;

	virtual void FinishPromotion(const FString& chr, FCGUndo& undo);
};
