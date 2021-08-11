// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "CGKing.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ACGKing : public ACGPiece
{
	GENERATED_BODY()

	const static FString KingFen;
public:

	TSet<ACGTile*> CastleTiles;

	ACGKing();

	const bool IsFenMatches(const TCHAR& iChr) const override;

	const FString GetFenChar() const override;

	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265A") : TEXT("\u2654")); }

	void MoveToTile(ACGTile* iTile) override;

	void MoveToTileInternal(ACGTile* iTile, FCGUndo& oUndo, bool iEvents) override;

	UFUNCTION()
	virtual bool IsInCheck();
};
