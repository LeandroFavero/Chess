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
	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265F") : TEXT("\u2659")); }//U+265F : U+2659

	//bool IsDoubleOpenAvailable();


	void BeginPromotion();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	void EndPromotion();
};
