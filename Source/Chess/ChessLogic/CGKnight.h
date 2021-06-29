// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPiece.h"
#include "CGKnight.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ACGKnight : public ACGPiece
{
	GENERATED_BODY()
public:
	ACGKnight();
	
	const FString GetFenChars() const override { return "Nn"; }

};
