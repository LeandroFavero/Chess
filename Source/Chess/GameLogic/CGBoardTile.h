// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChessLogic/CGSquareCoord.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGBoardTile.generated.h"

class ACGChessBoard;
class UCGLabelWidgetComponent;
class ACGPiece;

UENUM()
enum class EDir : uint8
{
	NORTH			,
	NORTH_EAST		,
	EAST			,
	SOUTH_EAST		,
	SOUTH			,
	SOUTH_WEST		,
	WEST			,
	NORTH_WEST		,
	KNIGHT1			,
	KNIGHT2			,
	KNIGHT3			,
	KNIGHT4			,
	KNIGHT5			,
	KNIGHT6			,
	KNIGHT7			,
	KNIGHT8			,
	Size
};

UCLASS()
class CHESS_API ACGBoardTile : public AActor
{
	GENERATED_BODY()

public:
	static const int NORTH{ 0 };
	static const int NORTH_EAST{ 1 };
	static const int EAST{ 2 };
	static const int SOUTH_EAST{ 3 };
	static const int SOUTH{ 4 };
	static const int SOUTH_WEST{ 5 };
	static const int WEST{ 6 };
	static const int NORTH_WEST{ 7 };
	static const int KNIGHT1{ 8 };
	static const int KNIGHT2{ 9 };
	static const int KNIGHT3{ 10 };
	static const int KNIGHT4{ 11 };
	static const int KNIGHT5{ 12 };
	static const int KNIGHT6{ 13 };
	static const int KNIGHT7{ 14 };
	static const int KNIGHT8{ 15 };

	UPROPERTY()
	TArray<ACGPiece*> AttackedBy;

	UPROPERTY(Replicated)
	TArray<ACGPiece*> OccupiedBy;

	UPROPERTY()
	bool m_isBlack{ false };

	UPROPERTY()
	TArray<ACGBoardTile*> Neighbours;

public:

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
	//TSubclassOf<class UCGLabelWidget> WidgetTemplate;
	TSubclassOf<class UCGLabelWidgetComponent> WidgetTemplate;

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FRotator WidgetRotation {0.0f, 90.0f, 0.0f};

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FVector WidgetOffset {100, 100, 1};

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
	virtual void SetCoord(const FCGSquareCoord coord);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void SetBlack(bool value);

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual bool IsBlack();

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void ClearAttackers();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
