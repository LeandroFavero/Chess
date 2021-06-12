// Fill out your copyright notice in the Description page of Project Settings.


#include "CGPiece.h"

// Sets default values
ACGPiece::ACGPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//register clickable
	//BlockMesh
	if (Mesh)
	{
		//Mesh->OnClicked.AddDynamic(this, &APuzzleTesztBlock::BlockClicked);
	}
}

// Called when the game starts or when spawned
void ACGPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACGPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACGPiece::MoveTo()
{

}

void ACGPiece::FillAttackMap()
{

}

void ACGPiece::Capture()
{

}

void ACGPiece::OnPieceCaptured_Implementation()
{

}

void ACGPiece::OnPieceMoved_Implementation()
{

}

void ACGPiece::OnPieceSpawned_Implementation()
{

}
