// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
class UCGAnimInstance;
class ACGChessPlayerController;
class ACGChessBoard;
class ACGBoardTile;
#include "ChessLogic/CGSquareCoord.h"
#include "CGPiece.generated.h"

UCLASS()
class CHESS_API ACGPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACGPiece();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	TSubclassOf<class UCGAnimInstance> AnimTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	UStaticMeshComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	bool IsBlack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	ACGChessPlayerController* OwningPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	FCGSquareCoord Position;

	UPROPERTY()
	ACGChessBoard* Board;

	UPROPERTY()
	ACGBoardTile* Tile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual const FString GetFenChars() const { return ""; }

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void SetMaterial(UMaterialInstance* mat);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void Grab(bool isGrabbed);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual bool UpdateGrab(/*ACGChessPlayerController* pc,*/FVector_NetQuantize Location);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void SnapToPlace();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void MoveTo(const FCGSquareCoord& coord, bool bypassCheck = false);

	/*
	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void MoveTo(ACGBoardTile* tile, bool bypassCheck = false);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void MoveTo(ACGPiece* piece, bool bypassCheck = false);
	*/

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual TSet<ACGBoardTile*> AvailableMoves();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void FillAttackMap();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void Capture();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Chess setup")
	void OnPieceGrabbed(bool isGrabbed);
	void OnPieceGrabbed_Implementation(bool isGrabbed);

	UFUNCTION(BlueprintNativeEvent, Category = "Chess events")
	void OnPieceCaptured();
	void OnPieceCaptured_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Chess events")
	void OnPieceMoved(FCGSquareCoord coord);
	void OnPieceMoved_Implementation(FCGSquareCoord coord);

	UFUNCTION(BlueprintNativeEvent, Category = "Chess events")
	void OnPieceSpawned();
	void OnPieceSpawned_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Chess events")
	void OnInvalidMove();
	void OnInvalidMove_Implementation();
};
