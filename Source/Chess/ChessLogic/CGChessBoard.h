// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameLogic/CGBoardTile.h"
#include "Math/IntPoint.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGChessBoard.generated.h"

UCLASS()
class CHESS_API ACGChessBoard : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	TSubclassOf<class ACGBoardTile> TileTemplate;

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FSquareCoord Size {8, 8};

	UPROPERTY()
	TArray<ACGBoardTile*> Board;


	USceneComponent* Root;
	// Sets default values for this actor's properties
	ACGChessBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void Destroyed() override;
};
