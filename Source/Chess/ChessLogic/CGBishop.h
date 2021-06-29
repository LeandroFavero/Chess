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
	const FString GetFenChars() const override { return "Bb"; }

};
