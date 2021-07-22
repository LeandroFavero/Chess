// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGLinearMovement.h"
#include "CGKingMovement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Chess), meta = (BlueprintSpawnableComponent))
class CHESS_API UCGKingMovement : public UCGLinearMovement
{
	GENERATED_BODY()

public:
	UCGKingMovement();

	void GetAvailableMoves(TSet<ACGTile*>& set) override;

	//void GetAttackedTiles(TSet<ACGTile*>& set) override;
};
