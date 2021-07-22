// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CGSquareCoord.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGPieceMovementBase.generated.h"

class ACGTile;

UCLASS(Abstract, ClassGroup=(Chess))
class CHESS_API UCGPieceMovementBase : public UActorComponent
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable, Category = "Chess logic")
	virtual void GetAvailableMoves(TSet<ACGTile*>& set) {};

	UFUNCTION(BlueprintCallable, Category = "Chess logic")
	virtual void GetAttackedTiles(TSet<ACGTile*>& set) {};
};
