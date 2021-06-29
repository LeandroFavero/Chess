// Fill out your copyright notice in the Description page of Project Settings.


#include "CGHighlightableComponent.h"

// Sets default values for this component's properties
UCGHighlightableComponent::UCGHighlightableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCGHighlightableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCGHighlightableComponent::SetHighlighted(bool value)
{
	//TArray<UActorComponent*> meshes = ;
	for (UActorComponent* m : GetOwner()->GetComponentsByClass(USkeletalMeshComponent::StaticClass()))
	{
		USkeletalMeshComponent* smc = Cast<USkeletalMeshComponent>(m);
		if (smc)
		{
			smc->SetRenderCustomDepth(value);
		}
	}
	for (UActorComponent* m : GetOwner()->GetComponentsByClass(UStaticMeshComponent::StaticClass()))
	{
		UStaticMeshComponent* smc = Cast<UStaticMeshComponent>(m);
		if (smc)
		{
			smc->SetRenderCustomDepth(value);
		}
	}
}

