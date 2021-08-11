// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "CGRook.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ACGRook : public ACGPiece
{
	GENERATED_BODY()

	const static FString RookFen;
public:

	ACGRook();

	const bool IsFenMatches(const TCHAR& iChr) const override;

	const FString GetFenChar() const override;

	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265C") : TEXT("\u2656")); }

	const bool IsValidForPromotion() const override { return true; }
};
