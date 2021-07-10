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

public:

	ACGQueen();

	const FString GetFenChars() const override { return "Qq"; }
	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265B") : TEXT("\u2655")); }
};
