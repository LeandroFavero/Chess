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

	const static FString PawnFen;
public:
	ACGPawn();

	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265F") : TEXT("\u2659")); }

	const bool IsFenMatches(const TCHAR& iChr) const override;

	const FString GetFenChar() const override;

	ACGTile* EnPassantTile { nullptr };

	void MoveToTileInternal(ACGTile* iTile, FCGUndo& oUndo, bool iEvents) override;

	virtual void FinishPromotion(const FString& iChr, FCGUndo& oUndo);
};
