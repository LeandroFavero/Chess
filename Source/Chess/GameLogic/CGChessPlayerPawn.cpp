// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CGBoardTile.h"
#include "CGHighlightableComponent.h"
#include "CGChessPlayerPawn.h"

// Sets default values
ACGChessPlayerPawn::ACGChessPlayerPawn() : Super()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(CameraArm);
	
	CameraArm->SetRelativeRotation(CameraArmDefaultRotation);
	CameraArm->TargetArmLength = CameraArmLengthDefault;

}

// Called when the game starts or when spawned
void ACGChessPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACGChessPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//calculate mouseover
	TraceCursor();
}

// Called to bind functionality to input
void ACGChessPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &ACGChessPlayerPawn::BeginTurnCamera).bConsumeInput = false;
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &ACGChessPlayerPawn::EndTurnCamera).bConsumeInput = false;

	PlayerInputComponent->BindAxis("MouseX", this, &ACGChessPlayerPawn::MouseMoveX);
	PlayerInputComponent->BindAxis("MouseY", this, &ACGChessPlayerPawn::MouseMoveY);
	PlayerInputComponent->BindAxis("Scroll", this, &ACGChessPlayerPawn::Zoom);
}

void ACGChessPlayerPawn::TraceCursor()
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		FVector start, dir, end;
		pc->DeprojectMousePositionToWorld(start, dir);
		end = start + (dir * 8000.0f);
		FHitResult hitResult;
		GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);
		if (bDrawDebugHelpers)
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red);
			DrawDebugSolidBox(GetWorld(), hitResult.Location, FVector(20.0f), FColor::Red);
		}
		if (hitResult.Actor != MouseoveredActor)
		{
			//disable old highlight
			SetMouseoverHighlighted(false);
			MouseoveredActor = hitResult.Actor;
			//enable new highlight
			SetMouseoverHighlighted(true);
			/*if (HitResult.Actor.IsValid())
			{
				ACGBoardTile* tile = Cast<ACGBoardTile>(MouseoveredActor.Get());
				UCGHighlightableComponent* highlight = MouseoveredActor.Get()->FindComponentByClass<UCGHighlightableComponent>();
				if (highlight)
				{
					highlight->SetHighlighted(true);
				}
				//CGPiece* piece = Cast<ACGPiece>(HitResult.Actor.Get());
			}*/
		}
	}
		
		//APuzzleTesztBlock* HitBlock = Cast<APuzzleTesztBlock>(HitResult.Actor.Get());
		/*if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}*/
	/*else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}*/
}

void ACGChessPlayerPawn::SetMouseoverHighlighted(bool value)
{
	if (MouseoveredActor.IsValid())
	{
		UCGHighlightableComponent* highlight = MouseoveredActor.Get()->FindComponentByClass<UCGHighlightableComponent>();
		if (highlight)
		{
			highlight->SetHighlighted(value);
		}
	}
}

void ACGChessPlayerPawn::BeginTurnCamera()
{
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		pc->bShowMouseCursor = false;
		pc->GetMousePosition(lastMouseX, lastMouseY);
		pc->SetInputMode(FInputModeGameOnly());
	}
	bIsAdjustingCamera = true;
}
void ACGChessPlayerPawn::EndTurnCamera()
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->bShowMouseCursor = true;
		pc->GetLocalPlayer()->ViewportClient->Viewport->SetMouse(lastMouseX, lastMouseY);
		pc->SetInputMode(FInputModeGameAndUI());
		/*if (ULocalPlayer* lp = Cast<ULocalPlayer>(pc->Player))
		{
			if (FViewport* v = lp->ViewportClient->Viewport)
			{
				v->SetMouse(lastMouseX, lastMouseY);
			}
		}*/
	}
	bIsAdjustingCamera = false;
}

void ACGChessPlayerPawn::Zoom(float Val)
{
	CameraArm->TargetArmLength += (Val * CameraArmZoomSpeed);
	if (CameraArm->TargetArmLength < CameraArmLengthMin)
	{
		CameraArm->TargetArmLength = CameraArmLengthMin;
	}
	else if (CameraArm->TargetArmLength > CameraArmLengthMax)
	{
		CameraArm->TargetArmLength = CameraArmLengthMax;
	}
}

void ACGChessPlayerPawn::MouseMoveX(float val)
{
	if (bIsAdjustingCamera)
	{
		OrbitCamera(val * CameraArmSpeed, 0);
	}
}

void ACGChessPlayerPawn::MouseMoveY(float val)
{
	if (bIsAdjustingCamera)
	{
		OrbitCamera(0, val * CameraArmSpeed);
	}
}

void ACGChessPlayerPawn::OrbitCamera(float X, float Y)
{
	FRotator rot = CameraArm->GetDesiredRotation();
	rot.Pitch += Y;
	rot.Yaw += X;
	if (rot.Pitch > CameraArmYMax)
	{
		rot.Pitch = CameraArmYMax;
	}
	else if (rot.Pitch < CameraArmYMin)
	{
		rot.Pitch = CameraArmYMin;
	}
	CameraArm->SetRelativeRotation(rot);
}