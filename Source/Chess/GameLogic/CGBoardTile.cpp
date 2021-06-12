// Fill out your copyright notice in the Description page of Project Settings.

#include "CGHighlightableComponent.h"
#include "CGBoardTile.h"

// Sets default values
ACGBoardTile::ACGBoardTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	UCGHighlightableComponent* highlight = CreateDefaultSubobject<UCGHighlightableComponent>(TEXT("Highlight"));
	AddOwnedComponent(highlight);
}

// Called when the game starts or when spawned
void ACGBoardTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACGBoardTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACGBoardTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DynaMats.Empty();
	for (int i = 0; i < Mesh->GetNumMaterials(); ++i)
	{
		auto dynaMat = Mesh->CreateDynamicMaterialInstance(i);
		DynaMats.Add(dynaMat);
	}
}

void ACGBoardTile::SetBlack(bool value)
{
	m_isBlack = value;
	for (UMaterialInstanceDynamic* mat : DynaMats)
	{
		if (value)
		{
			mat->SetScalarParameterValue(FName("IsBlack"), 1.0f );
		}
		else
		{
			mat->SetScalarParameterValue(FName("IsBlack"), 0.0f);
		}
	}
}

bool ACGBoardTile::IsBlack()
{
	return m_isBlack;
}
