// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "CGPawnMovement.generated.h"

class ACGBoardTile;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Chess), meta = (BlueprintSpawnableComponent))
class CHESS_API UCGPawnMovement : public UCGPieceMovementBase
{
	GENERATED_BODY()

	void AvailableMoves(TSet<ACGBoardTile*>& set) override;

	void AttackedTiles(TSet<ACGBoardTile*>& set) override;
};
