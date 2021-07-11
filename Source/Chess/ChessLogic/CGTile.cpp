// Fill out your copyright notice in the Description page of Project Settings.

#include "CGTile.h"
#include "ChessLogic/CGChessBoard.h"
#include "GameLogic/CGHighlightableComponent.h"
#include "UI/CGLabelWidgetComponent.h"
#include "ChessLogic/CGPiece.h"
#include "Net/UnrealNetwork.h"

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

// Sets default values
ACGTile::ACGTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("BoardCollision"), false);
	SetRootComponent(Mesh);

	UCGHighlightableComponent* highlight = CreateDefaultSubobject<UCGHighlightableComponent>(TEXT("Highlight"));
	AddOwnedComponent(highlight);

	//Neighbours.Reserve(16);
	Neighbours.SetNumZeroed(static_cast<int>(EDir::Size));

	bReplicates = true;
	bOnlyRelevantToOwner = false;
}

// Called when the game starts or when spawned
void ACGTile::BeginPlay()
{
	Super::BeginPlay();
	SetCoord(Position);
}

// Called every frame
void ACGTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACGTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ACGTile::SetCoord(const FCGSquareCoord coord)
{
	Position = coord;
	SetBlack(coord.X % 2 == coord.Y % 2);
	if (!Board || !WidgetTemplate || !GetWorld())
	{
		return;
	}
	FString newName = FString::Printf(TEXT("Tile_%dx%d_"), coord.X, coord.Y);
	//Rename(*newName);
	SetActorLabel(*newName);
	

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

void ACGTile::SetBlack(bool value)
{
	m_isBlack = value;
	Mesh->SetMaterial(0, m_isBlack? Black : White);
}

bool ACGTile::IsBlack()
{
	return m_isBlack;
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