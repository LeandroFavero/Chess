// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessPlayerPawn.h"
#include "Engine/World.h"
#include "ChessLogic/CGPiece.h"
#include "DrawDebugHelpers.h"
#include "ChessLogic/CGTile.h"
#include "CGChessPlayerController.h"
#include "UI/CGHUD.h"
#include "CGHighlightableComponent.h"
#include "GameLogic/CGGameState.h"
#include "GameLogic/CGGameMode.h"
#include "Blueprint/CGBPUtils.h"
#include "ChessLogic/CGChessBoard.h"

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

	SetRootComponent(CameraArm);

	bOnlyRelevantToOwner = true;
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void ACGChessPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	if (ACGChessPlayerController* pc = GetWorld()->GetFirstPlayerController<ACGChessPlayerController>())
	{
		pc->OnMove.AddDynamic(this, &ACGChessPlayerPawn::OnMove);
		pc->OnStart.AddDynamic(this, &ACGChessPlayerPawn::OnStart);
	}
}

void ACGChessPlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ACGChessPlayerController* pc = GetWorld()->GetFirstPlayerController<ACGChessPlayerController>())
	{
		pc->OnMove.RemoveDynamic(this, &ACGChessPlayerPawn::OnMove);
		pc->OnStart.RemoveDynamic(this, &ACGChessPlayerPawn::OnStart);
	}
	Super::EndPlay(EndPlayReason);
}

void ACGChessPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled()) 
	{
		if (bIsSpinnyMenu)
		{
			OrbitCamera(SpinnyMenuSpeed * DeltaTime, 0, false);
		}
		else
		{
			TraceCursor();
		}
	}
}

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
	if (ACGChessPlayerController* pc = GetController<ACGChessPlayerController>())
	{
		if (bIsAdjustingCamera)
		{
			EndGrabPiece();
			return;
		}
		//collision disabled for pieces if one is lifted
		FHitResult hitResult;
		pc->GetHitResultUnderCursor(GrabbedPiece.IsValid() ? ECC_GameTraceChannel1 : ECC_Visibility, false, hitResult);
		if (bDrawDebugHelpers)
		{
			DrawDebugSolidBox(GetWorld(), hitResult.Location, FVector(20.0f), FColor::Red);
		}
		SetMouseovered(hitResult.GetActor());
		if (GrabbedPiece.IsValid())
		{
			pc->ServerUpdateGrab(GrabbedPiece.Get(), hitResult.Location);
		}
	}
}

void ACGChessPlayerPawn::OnMove()
{
	SetMouseovered(nullptr);
}

void ACGChessPlayerPawn::OnStart()
{
	if (ACGChessPlayerController* pc = GetWorld()->GetFirstPlayerController<ACGChessPlayerController>())
	{
		OrbitCamera(pc->bIsBlack ? BlackRotation : WhiteRotation, CameraArmYDefault, true);
	}
}

bool ACGChessPlayerPawn::IsPieceInteractable(const ACGPiece* iPiece, const ACGChessPlayerController* iPc)
{
	return (iPc && iPiece->Board && iPiece->Board->IsReadyForNextMove() &&
		(iPiece->IsBlack() == iPc->bIsBlack || UCGBPUtils::IsHotSeatMode(this)) &&
		(iPiece->Board->IsNextMoveBlack() == iPiece->IsBlack()) && !bIsSpinnyMenu);
}

void ACGChessPlayerPawn::BeginTurnCamera()
{
	bIsAdjustingCamera = true;
	EndGrabPiece();
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->bShowMouseCursor = false;
		pc->GetMousePosition(lastMouseX, lastMouseY);
	}
	bIsAdjustingCamera = true;
}

void ACGChessPlayerPawn::EndTurnCamera()
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->GetLocalPlayer()->ViewportClient->Viewport->SetMouse(lastMouseX, lastMouseY);
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
	if (ACGChessPlayerController* pc = GetController<ACGChessPlayerController>())
	{
		if (GrabbedPiece.IsValid())
		{
			pc->ServerGrab(GrabbedPiece.Get(), false);
		}
		if (MouseoveredActor.IsValid())
		{
			if (ACGPiece* piece = Cast<ACGPiece>(MouseoveredActor.Get()))
			{
				if (piece->IsCaptured())
				{
					return;
				}
				//can we move it?
				if (IsPieceInteractable(piece, pc))
				{
					GrabbedPiece = piece;
					pc->ServerGrab(piece, true);

					HighlightedTiles = piece->GetAvailableMoves();
					HighlightTiles(true);
				}
			}
		}
	}
}

void ACGChessPlayerPawn::EndGrabPiece(bool moveTo)
{
	HighlightTiles(false);
	if (ACGChessPlayerController* pc = GetController<ACGChessPlayerController>())
	{
		if (moveTo)
		{
			pc->ServerMoveToTile(GrabbedPiece.Get(), Cast<ACGTile>(MouseoveredActor.Get()));
		}
		if (GrabbedPiece.IsValid())
		{
			pc->ServerGrab(GrabbedPiece.Get(), false);
			GrabbedPiece = nullptr;
		}
	}
}

void ACGChessPlayerPawn::HighlightTiles(bool val)
{
	for (AActor* t : HighlightedTiles)
	{
		if (t)
		{
			if (UCGHighlightableComponent* highlight = t->FindComponentByClass<UCGHighlightableComponent>())
			{
				highlight->SetHighlighted(val);
			}
		}
	}
}

void ACGChessPlayerPawn::SetMouseovered(const TWeakObjectPtr<class AActor>& iActor)
{
	if (iActor != MouseoveredActor)
	{
		//remove highlight if it's a piece
		if (ACGPiece* p = Cast<ACGPiece>(MouseoveredActor))
		{
			if (UCGHighlightableComponent* highlight = p->FindComponentByClass<UCGHighlightableComponent>())
			{
				highlight->SetHighlighted(false);
			}
		}
		MouseoveredActor = iActor;
		//highlight piece if it has valid moves
		if (ACGPiece* p = Cast<ACGPiece>(MouseoveredActor))
		{
			if (ACGChessPlayerController* pc = GetController<ACGChessPlayerController>())
			{
				if (IsPieceInteractable(p, pc) && p->HasAvailableMoves())
				{
					if (UCGHighlightableComponent* highlight = p->FindComponentByClass<UCGHighlightableComponent>())
					{
						highlight->SetHighlighted(true);
					}
				}
			}
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