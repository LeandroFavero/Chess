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

	const static FString KnightFen;
public:

	UPROPERTY(EditAnywhere, Category="Chess setup")
	float BlackMeshRotation = 90;

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	float WhiteMeshRotation = 270;

	ACGKnight();

	const bool IsFenMatches(const TCHAR& iChr) const override;

	const FString GetFenChar() const override;

	const FString GetUnicode() const override { return (IsBlack() ? TEXT("\u265E") : TEXT("\u2658")); }

	const bool IsValidForPromotion() const override { return true; }

	void RefreshMaterial() override;
};
