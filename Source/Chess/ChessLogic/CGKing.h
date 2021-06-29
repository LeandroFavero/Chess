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
	
	const FString GetFenChars() const override { return "Kk"; }
};
