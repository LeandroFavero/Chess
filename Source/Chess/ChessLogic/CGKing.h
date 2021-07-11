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
public:
	ACGKing();
	const FString GetFenChars() const override { return "Kk"; }
	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265A") : TEXT("\u2654")); }

	TSet<ACGTile*> CastleTiles;
	//= nullptr;
	//ACGTile* CastleEast = nullptr;

	void MoveToTileInternal(ACGTile* pTile, FCGUndo& undo, bool pEvents) override;

	TSet<ACGTile*> AvailableMoves() override;

	UFUNCTION()
	virtual bool IsInCheck();
};
