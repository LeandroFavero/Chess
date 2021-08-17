// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGChessPlayerPawn.generated.h"

class ACGPiece;
class ACGTile;
class ACGChessPlayerController;

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
	bool bIsAdjustingCamera = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsLeftClicking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bIsSpinnyMenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float SpinnyMenuSpeed = -10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthDefault = 2500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthMin = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmLengthMax = 2500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraArmZoomSpeed = -50.0f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugHelpers;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class ACGPiece> GrabbedPiece;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class AActor> MouseoveredActor;

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	float DragTreshold = 3;

	float lastMouseX, lastMouseY;

	ACGChessPlayerPawn();

private:
	void LeftPressed();
	void LeftReleased();
	void BeginGrabPiece();
	void EndGrabPiece(bool iIsMoveTo = false);
	bool IsDragged();
	void BeginTurnCamera();
	void EndTurnCamera();
	void MouseMoveX(float iVal);
	void MouseMoveY(float iVal);
	void Zoom(float iVal);
	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	void OrbitCamera(float iX, float iY, bool iIsAbsolute = false);
	void HighlightTiles(bool iIsHighlighted);
	void SetMouseovered(const TWeakObjectPtr<class AActor>& iActor);

	UPROPERTY()
	TSet<ACGTile*> HighlightedTiles;

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TraceCursor();

	UFUNCTION()
	void OnMove();
	UFUNCTION()
	void OnStart();
	bool IsPieceInteractable(const ACGPiece* iPiece, const ACGChessPlayerController* iPc);
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
