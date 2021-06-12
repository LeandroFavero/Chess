// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChessLogic/SquareCoord.h"
#include "ChessLogic/CGPiece.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGBoardTile.generated.h"

UCLASS()
class CHESS_API ACGBoardTile : public AActor
{
	GENERATED_BODY()
	
private:
	TArray<ACGPiece> AttackedBy;
	TArray<ACGPiece> OccupiedBy;

	UPROPERTY()
	bool m_isBlack{ false };

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynaMats;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	FVector2D Size {100, 100};

	UPROPERTY(BlueprintReadOnly, Category = "Chess setup")
	FSquareCoord Position;

	// Sets default values for this actor's properties
	ACGBoardTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void SetBlack(bool value);

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual bool IsBlack();
};
