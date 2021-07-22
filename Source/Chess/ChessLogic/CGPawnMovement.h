// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "CGPawnMovement.generated.h"

class ACGTile;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Chess), meta = (BlueprintSpawnableComponent))
class CHESS_API UCGPawnMovement : public UCGPieceMovementBase
{
	GENERATED_BODY()
public:
	void GetAvailableMoves(TSet<ACGTile*>& oSet) override;

	void GetAttackedTiles(TSet<ACGTile*>& oSet) override;
};
