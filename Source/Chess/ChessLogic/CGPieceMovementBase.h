// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CGSquareCoord.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGPieceMovementBase.generated.h"

class ACGTile;

UCLASS(Blueprintable, ClassGroup=(Chess))
class CHESS_API UCGPieceMovementBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCGPieceMovementBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION(BlueprintPure, Category="Chess logic")
	//virtual bool IsMoveValid(const ACGTile* pTile);

	UFUNCTION(BlueprintCallable, Category = "Chess logic")
	virtual void AvailableMoves(TSet<ACGTile*>& set);

	UFUNCTION(BlueprintCallable, Category = "Chess logic")
	virtual void AttackedTiles(TSet<ACGTile*>& set);
};
