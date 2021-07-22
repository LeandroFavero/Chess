// Fill out your copyright notice in the Description page of Project Settings.

#include "CGTile.h"
#include "ChessLogic/CGChessBoard.h"
#include "GameLogic/CGHighlightableComponent.h"
#include "UI/CGLabelWidgetComponent.h"
#include "ChessLogic/CGPiece.h"
#include "Net/UnrealNetwork.h"

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

ACGTile::ACGTile()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("BoardCollision"), false);
	SetRootComponent(Mesh);

	UCGHighlightableComponent* highlight = CreateDefaultSubobject<UCGHighlightableComponent>(TEXT("Highlight"));
	AddOwnedComponent(highlight);

	Neighbours.SetNumZeroed(static_cast<int>(EDir::Size));

	bReplicates = true;
	bOnlyRelevantToOwner = false;
}

void ACGTile::BeginPlay()
{
	Super::BeginPlay();
	SetCoord(Position);
}

void ACGTile::SetCoord(const FCGSquareCoord iCoord)
{
	Position = iCoord;
	SetBlack(iCoord.X % 2 == iCoord.Y % 2);
	if (!Board || !WidgetTemplate || !GetWorld())
	{
		return;
	}

#if WITH_EDITOR
	FString newName = FString::Printf(TEXT("Tile_%dx%d_"), iCoord.X, iCoord.Y);
	SetActorLabel(*newName);
#endif

	bool horizontal = Position.X == 0 || Position.X == (Board->Size.X - 1);
	bool vertical = Position.Y == 0 || Position.Y == (Board->Size.Y - 1);
	TArray<UCGLabelWidgetComponent*> comps;
	GetComponents<UCGLabelWidgetComponent>(comps);
	if (!horizontal && !vertical)
	{
		for (auto it = comps.CreateIterator(); it; ++it)
		{
			if (*it)
			{
				(*it)->DestroyComponent();
				it.RemoveCurrent();
			}
		}
		return;
	}
	int labelNum = horizontal && vertical ? 2 : 1;
	while (comps.Num() > labelNum)
	{
		comps[comps.Num() - 1]->DestroyComponent();
		comps.RemoveAt(comps.Num() - 1);
	}
	while (comps.Num() < labelNum)
	{
		UCGLabelWidgetComponent* newComponent = NewObject<UCGLabelWidgetComponent>(this, WidgetTemplate);
		AddInstanceComponent(newComponent);
		newComponent->RegisterComponent();
		newComponent->SetRelativeRotation(WidgetRotation);
		newComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
		
		newComponent->InitWidget();
		comps.Add(newComponent);
	}
	int idx = 0;
	constexpr int DIGIT_FLAG = 1 << 31;
	if (Position.X == 0)
	{
		//Left
		UCGLabelWidgetComponent* widgetComp = comps[idx];
		widgetComp->SetRelativeRotation(WidgetRotation + FRotator(0, 90, 0));
		widgetComp->SetRelativeLocation(WidgetOffset.RotateAngleAxis(90, FVector(0.0f, 0.0f, 1.0f)));
		widgetComp->SetLabel(DIGIT_FLAG | Position.Y+1);
		idx += 1;
	}
	if (Position.X == (Board->Size.X - 1))
	{
		//Right
		UCGLabelWidgetComponent* widgetComp = comps[idx];
		widgetComp->SetRelativeRotation(WidgetRotation + FRotator(0, 270,0));
		widgetComp->SetRelativeLocation(WidgetOffset.RotateAngleAxis(270, FVector(0.0f, 0.0f, 1.0f)));
		widgetComp->SetLabel(DIGIT_FLAG | Position.Y+1);
		idx += 1;
	}

	if (Position.Y == 0 )
	{
		//Bottom
		UCGLabelWidgetComponent* widgetComp = comps[idx];
		widgetComp->SetRelativeRotation(WidgetRotation + FRotator(0, 90, 0));
		widgetComp->SetRelativeLocation(WidgetOffset.RotateAngleAxis(0, FVector(0.0f, 0.0f, 1.0f)));
		widgetComp->SetLabel(Position.X);
		idx += 1;
	}
	if (Position.Y == (Board->Size.Y - 1))
	{
		//Top
		UCGLabelWidgetComponent* widgetComp = comps[idx];
		widgetComp->SetRelativeRotation(WidgetRotation + FRotator(0, 270, 0));
		widgetComp->SetRelativeLocation(WidgetOffset.RotateAngleAxis(180, FVector(0.0f, 0.0f, 1.0f)));
		widgetComp->SetLabel(Position.X);
		idx += 1;
	}
}

void ACGTile::SetBlack(bool iIsBlack)
{
	bIsBlack = iIsBlack;
	Mesh->SetMaterial(0, bIsBlack? Black : White);
}

bool ACGTile::IsBlack()
{
	return bIsBlack;
}

void ACGTile::ClearAttackers()
{
	AttackedBy.Empty();
}

void ACGTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGTile, OccupiedBy)
}