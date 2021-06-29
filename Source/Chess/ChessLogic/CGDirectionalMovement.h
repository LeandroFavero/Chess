// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "CGDirectionalMovement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Chess), meta = (BlueprintSpawnableComponent))
class CHESS_API UCGDirectionalMovement : public UCGPieceMovementBase
{
	GENERATED_BODY()
	
	////UFUNCTION(BlueprintPure, Category = "Chess logic")
	//bool IsMoveValid(const FCGSquareCoord& coord) override;

	//void AttackedTiles(TSet<ACGBoardTile*>& set) override;
};
