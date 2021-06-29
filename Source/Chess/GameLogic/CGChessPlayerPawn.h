// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
class ACGPiece;
class ACGBoardTile;

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGChessPlayerPawn.generated.h"

UENUM(BlueprintType)
enum class ESide: uint8
{
	Spectator	UMETA(DisplayName = "Spectator"),
	Black	UMETA(DisplayName = "Black"),
	White	UMETA(DisplayName = "White")
};

UCLASS()
class CHESS_API ACGChessPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FRotator CameraArmDefaultRotation {-45.0f, 0.0f, 0.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmYMin = -85.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmYDefault = -45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmYMax = -10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BlackRotation = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float WhiteRotation = 270.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float RotationDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmSpeed = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsAdjustingCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsLeftClicking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthDefault = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthMin = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthMax = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmZoomSpeed = -50.0f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugHelpers;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class ACGPiece> GrabbedPiece{};

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class AActor> MouseoveredActor{};


	UPROPERTY(EditAnywhere, Category = "Chess setup")
	float DragTreshold = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Debug")//TODO: remove?
	FHitResult hitResult;

	float lastMouseX, lastMouseY;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	ESide Side;

	// Sets default values for this pawn's properties
	ACGChessPlayerPawn();

private:
	void LeftPressed();
	void LeftReleased();
	void BeginGrabPiece();
	void EndGrabPiece(bool moveTo = false);
	bool IsDragged();
	void BeginTurnCamera();
	void EndTurnCamera();
	void MouseMoveX(float Val);
	void MouseMoveY(float Val);
	void Zoom(float Val);
	void OrbitCamera(float X, float Y, bool absolute = false);
	void HighlightTiles(bool val);

	UPROPERTY()
	TSet<ACGBoardTile* > HighlightedTiles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TraceCursor();
	virtual void SetMouseoverHighlighted(bool value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
