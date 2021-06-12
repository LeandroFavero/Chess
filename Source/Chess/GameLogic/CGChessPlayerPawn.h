// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChessLogic/CGPiece.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGChessPlayerPawn.generated.h"

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
	float CameraArmYMax = -10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmSpeed = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsAdjustingCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthDefault = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthMin = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthMax = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmZoomSpeed = -25.0f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugHelpers;

	UPROPERTY()
	TWeakObjectPtr<class AActor> MouseoveredActor{};

	UPROPERTY()
	ACGPiece* PickedUpPiece{};
	float lastMouseX, lastMouseY;
	// Sets default values for this pawn's properties
	ACGChessPlayerPawn();

private:
	void BeginTurnCamera();
	void EndTurnCamera();
	void MouseMoveX(float Val);
	void MouseMoveY(float Val);
	void Zoom(float Val);
	void OrbitCamera(float X, float Y);

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
