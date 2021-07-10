// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "GameLogic/CGUndo.h"
#include "CGPawn.generated.h"

class ACGBoardTile;
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

	ACGBoardTile* EnPassantTile = nullptr;

	void BeginPromotion();

	void MoveToTileInternal(ACGBoardTile* pTile, FCGUndo& undo, bool pEvents) override;

	UFUNCTION(BlueprintCallable, Category = "Chess")
	void EndPromotion();
};
