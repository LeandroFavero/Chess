// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "CGKingMovement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Chess), meta = (BlueprintSpawnableComponent))
class CHESS_API UCGKingMovement : public UCGPieceMovementBase
{
	GENERATED_BODY()

public:
	void AvailableMoves(TSet<ACGTile*>& set) override;

	void AttackedTiles(TSet<ACGTile*>& set) override;
};
