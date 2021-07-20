// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "CGBishop.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ACGBishop : public ACGPiece
{
	GENERATED_BODY()

public:
	ACGBishop();
	const FString GetFenChars() const override { return "Bb"; }
	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265D") : TEXT("\u2657")); }
	const bool IsValidForPromotion() const override { return true; }
};
