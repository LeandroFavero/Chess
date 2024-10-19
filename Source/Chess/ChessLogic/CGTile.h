// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChessLogic/CGSquareCoord.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGTile.generated.h"

class ACGChessBoard;
class UCGLabelWidgetComponent;
class ACGPiece;

//Directions on the board from the white player's pov
UENUM(BlueprintType)
enum EDir
{
	NORTH		=0,
	NORTH_EAST	=1,
	EAST		=2,
	SOUTH_EAST	=3,
	SOUTH		=4,
	SOUTH_WEST	=5,
	WEST		=6,
	NORTH_WEST	=7,
	KNIGHT1		=8,
	KNIGHT2		=9,
	KNIGHT3		=10,
	KNIGHT4		=11,
	KNIGHT5		=12,
	KNIGHT6		=13,
	KNIGHT7		=14,
	KNIGHT8		=15,
	Size
};

UCLASS()
class CHESS_API ACGTile : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsBlack = false;
public:

	UPROPERTY()
	TArray<ACGPiece*> AttackedBy;

	UPROPERTY(Replicated)
	ACGPiece* OccupiedBy = nullptr;

	UPROPERTY()
	TArray<ACGTile*> Neighbours;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	UMaterialInstance* Black;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	UMaterialInstance* White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, Category = "Chess setup")
	FCGSquareCoord Position;

	UPROPERTY(BlueprintReadOnly, Category = "Chess setup")
	ACGChessBoard* Board;

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	TSubclassOf<class UCGLabelWidgetComponent> WidgetTemplate;

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FRotator WidgetRotation {0.0f, 0.0f, 90.0f};

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FVector WidgetOffset {0, 165, 1};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ACGTile();

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void SetCoord(const FCGSquareCoord& Coord);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void SetBlack(bool bNewIsBlack);

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual bool IsBlack();

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void ClearAttackers();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void AppendCoordToString(FString& oStr, bool iIsCaps = true) const;
};
