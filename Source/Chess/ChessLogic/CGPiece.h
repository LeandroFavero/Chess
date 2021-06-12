// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGPiece.generated.h"

UCLASS()
class CHESS_API ACGPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACGPiece();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	UStaticMesh* Mesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void MoveTo();
	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void FillAttackMap();
	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void Capture();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Chess events")
	void OnPieceCaptured();
	void OnPieceCaptured_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Chess events")
	void OnPieceMoved();
	void OnPieceMoved_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Chess events")
	void OnPieceSpawned();
	void OnPieceSpawned_Implementation();
};
