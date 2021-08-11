// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "CGQueen.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ACGQueen : public ACGPiece
{
	GENERATED_BODY()

	const static FString QueenFen; 
public:

	ACGQueen();

	const bool IsFenMatches(const TCHAR& iChr) const override;

	const FString GetFenChar() const override;

	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265B") : TEXT("\u2655")); }

	const bool IsValidForPromotion() const override { return true; }
};
