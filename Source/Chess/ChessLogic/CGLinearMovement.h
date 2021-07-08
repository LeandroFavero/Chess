// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "GameLogic/CGBoardTile.h"
#include "CGLinearMovement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Chess), meta = (BlueprintSpawnableComponent))
class CHESS_API UCGLinearMovement : public UCGPieceMovementBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess logic")
	int Range{ -1 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess logic")
	TSet<EDir> Directions;

	void AvailableMoves(TSet<ACGBoardTile*>& set) override;

};
