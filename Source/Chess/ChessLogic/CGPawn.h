// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "CGPawn.generated.h"

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

	bool IsDoubleOpenAvailable();
};
