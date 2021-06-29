// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessPlayerPawn.h"
#include "Engine/World.h"
#include "ChessLogic/CGPiece.h"
#include "DrawDebugHelpers.h"
#include "CGBoardTile.h"

#include "CGHighlightableComponent.h"
#include "ChessGameMode.h"

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
	CameraArm->bDoCollisionTest = false;
}

// Called when the game starts or when spawned
void ACGChessPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	OrbitCamera(WhiteRotation, CameraArmYDefault, true);
}

// Called every frame
void ACGChessPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//calculate mouseover
	TraceCursor();
}

// Called to bind functionality to input
void ACGChessPlayerPawn::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);
	check(playerInputComponent);
	playerInputComponent->BindAction("RightClick", IE_Pressed, this, &ACGChessPlayerPawn::BeginTurnCamera).bConsumeInput = false;
	playerInputComponent->BindAction("RightClick", IE_Released, this, &ACGChessPlayerPawn::EndTurnCamera).bConsumeInput = false;
	playerInputComponent->BindAction("LeftClick", IE_Pressed, this, &ACGChessPlayerPawn::LeftPressed).bConsumeInput = false;
	playerInputComponent->BindAction("LeftClick", IE_Released, this, &ACGChessPlayerPawn::LeftReleased).bConsumeInput = false;

	playerInputComponent->BindAxis("MouseX", this, &ACGChessPlayerPawn::MouseMoveX);
	playerInputComponent->BindAxis("MouseY", this, &ACGChessPlayerPawn::MouseMoveY);
	playerInputComponent->BindAxis("Scroll", this, &ACGChessPlayerPawn::Zoom);
}

void ACGChessPlayerPawn::TraceCursor()
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		if (bIsAdjustingCamera)
		{
			SetMouseoverHighlighted(false);
			EndGrabPiece();
			return;
		}

		FVector start, dir, end;
		pc->DeprojectMousePositionToWorld(start, dir);
		end = start + (dir * 8000.0f);
		
		FCollisionQueryParams collisionParams;
		if (GrabbedPiece.IsValid())
		{
			collisionParams.AddIgnoredActor(GrabbedPiece.Get());
		}
		//FHitResult hitResult;
		GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams);
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

		}

		if (GrabbedPiece.IsValid())
		{
			GrabbedPiece.Get()->UpdateGrab(hitResult.Location);
		}
	}
}

void ACGChessPlayerPawn::SetMouseoverHighlighted(bool value)
{
	/*
	if (MouseoveredActor.IsValid())
	{
		UCGHighlightableComponent* highlight = MouseoveredActor.Get()->FindComponentByClass<UCGHighlightableComponent>();
		if (highlight)
		{
			highlight->SetHighlighted(value);
		}
	}
	*/
}

void ACGChessPlayerPawn::BeginTurnCamera()
{
	bIsAdjustingCamera = true;
	EndGrabPiece();
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->bShowMouseCursor = false;
		pc->GetMousePosition(lastMouseX, lastMouseY);
		//pc->SetInputMode(FInputModeGameOnly());
	}
	bIsAdjustingCamera = true;
}

void ACGChessPlayerPawn::EndTurnCamera()
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		
		pc->GetLocalPlayer()->ViewportClient->Viewport->SetMouse(lastMouseX, lastMouseY);
		//pc->SetInputMode(FInputModeGameOnly());
		pc->bShowMouseCursor = true;
		pc->bEnableClickEvents = true;
		pc->bEnableTouchEvents = true;
	}
	bIsAdjustingCamera = false;
}

void ACGChessPlayerPawn::LeftPressed()
{
	bIsLeftClicking = true;
	if (bIsAdjustingCamera)
	{
		EndTurnCamera();
	}
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->GetMousePosition(lastMouseX, lastMouseY);
	}
	if (GrabbedPiece.IsValid())
	{
		EndGrabPiece(true);
	}
	else
	{
		BeginGrabPiece();
	}
}

void ACGChessPlayerPawn::LeftReleased()
{
	bIsLeftClicking = false;
	if (bIsAdjustingCamera)
	{
		EndTurnCamera();
	}
	if (GrabbedPiece.IsValid() && IsDragged())
	{
		EndGrabPiece(true);
	}
}

bool ACGChessPlayerPawn::IsDragged()
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		float x, y;
		pc->GetMousePosition(x, y);
		float dist = FMath::Sqrt(FMath::Square(lastMouseX - x) + FMath::Square(lastMouseY - y));
		return dist > DragTreshold;
	}
	return false;
}

void ACGChessPlayerPawn::BeginGrabPiece()
{
	if (GrabbedPiece.IsValid())
	{
		GrabbedPiece.Get()->Grab(false);
	}
	if (MouseoveredActor.IsValid())
	{
		if (ACGPiece* piece = Cast<ACGPiece>(MouseoveredActor.Get()))
		{
			GrabbedPiece = piece;
			piece->Grab(true);
			
			HighlightedTiles = piece->AvailableMoves();
			HighlightTiles(true);
		}
	}
}

void ACGChessPlayerPawn::EndGrabPiece(bool moveTo)
{
	HighlightTiles(false);
	HighlightedTiles.Empty();
	if (moveTo)
	{
		if (UWorld* w = GetWorld())
		{
			if (AChessGameMode* mode = Cast<AChessGameMode>(w->GetAuthGameMode()))
			{
				GrabbedPiece.Get()->MoveTo(mode->Board->LocationToCoord(hitResult.Location));
			}
		}
	}
	if (GrabbedPiece.IsValid())
	{
		GrabbedPiece.Get()->Grab(false);
		GrabbedPiece = nullptr;
	}
}

void ACGChessPlayerPawn::HighlightTiles(bool val)
{
	for (ACGBoardTile* t : HighlightedTiles)
	{
		UCGHighlightableComponent* highlight = MouseoveredActor.Get()->FindComponentByClass<UCGHighlightableComponent>();
		if (highlight)
		{
			highlight->SetHighlighted(val);
		}
	}
}

void ACGChessPlayerPawn::Zoom(float val)
{
	if (val == 0)
	{
		return;
	}
	CameraArm->TargetArmLength += (val * CameraArmZoomSpeed);
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

void ACGChessPlayerPawn::OrbitCamera(float X, float Y, bool absolute)
{
	FRotator rot = CameraArm->GetDesiredRotation();
	if(absolute)
	{
		rot.Pitch = Y;
		rot.Yaw = X;
	}
	else
	{
		rot.Pitch += Y;
		rot.Yaw += X;
	}
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