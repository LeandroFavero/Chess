// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessBoard.h"
#include "SquareCoord.h"

// Sets default values
ACGChessBoard::ACGChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

// Called when the game starts or when spawned
void ACGChessBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACGChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACGChessBoard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	int32 tileCount = Size.X * Size.Y;

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(false, children);

	//gather the tile child actor components
	TArray<UChildActorComponent*> cacTiles;
	for (USceneComponent* c : children)
	{
		UChildActorComponent* cac = Cast<UChildActorComponent>(c);
		if (cac && cac->GetChildActorClass()->IsChildOf<ACGBoardTile>())
		{
			cacTiles.Add(cac);
		}
	}

	//add or delete to match the tile count
	for (int32 i = cacTiles.Num()-1; i > tileCount; --i)
	{
		cacTiles[i]->DestroyComponent();
		cacTiles.RemoveAt(i);
	}
	for (int32 i = cacTiles.Num(); i < tileCount; ++i)
	{
		UChildActorComponent* newCac = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass(), NAME_None);
		newCac->SetChildActorClass(TileTemplate);
		newCac->SetupAttachment(Root);
		newCac->RegisterComponent();
		cacTiles.Add(newCac);
	}

	//set up the existing tiles
	for (int32 i = 0; i < tileCount; ++i)
	{
		UChildActorComponent* cac = cacTiles[i];
		if (cac) 
		{
			ACGBoardTile* tile = Cast<ACGBoardTile>(cac->GetChildActor());
			if (tile)
			{
				uint8 x = i / Size.Y;
				uint8 y = i % Size.Y;
				tile->Position.X = x;
				tile->Position.Y = y;
				tile->SetBlack(x % 2 == y % 2);

				cac->SetRelativeLocation(FVector(x*tile->Size.X, y*tile->Size.Y, 0));
			}
		}
	}
}

void ACGChessBoard::Destroyed()
{
	Super::Destroyed();
}

