// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGPieceMovementBase.h"
#include "ChessLogic/CGTile.h"
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
	TSet<TEnumAsByte<EDir>> Directions;

	void GetAvailableMoves(TSet<ACGTile*>& oSet) override;

	void GetAttackedTiles(TSet<ACGTile*>& oSet) override;

private:
	void Generate(TSet<ACGTile*>& oSet, bool iIsAttack);
};
