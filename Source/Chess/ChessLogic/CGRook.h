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
	
public:

	ACGRook();

	const FString GetFenChars() const override { return "Rr"; }
	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265C") : TEXT("\u2656")); }
};
