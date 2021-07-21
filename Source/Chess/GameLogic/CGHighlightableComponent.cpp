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

void UCGHighlightableComponent::SetHighlighted(bool value)
{
	TArray<USkeletalMeshComponent*> skcs;
	GetOwner()->GetComponents<USkeletalMeshComponent>(skcs);
	for (USkeletalMeshComponent* m : skcs)
	{
		m->SetRenderCustomDepth(value);
	}
	TArray<UStaticMeshComponent*> sms;
	GetOwner()->GetComponents<UStaticMeshComponent>(sms);
	for (UStaticMeshComponent* m : sms)
	{
		if (m)
		{
			m->SetRenderCustomDepth(value);
		}
	}
}

