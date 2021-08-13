// Fill out your copyright notice in the Description page of Project Settings.


#include "CGHighlightableComponent.h"

UCGHighlightableComponent::UCGHighlightableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCGHighlightableComponent::SetHighlighted(bool value)
{
	TArray<USkeletalMeshComponent*> skcs;
	GetOwner()->GetComponents<USkeletalMeshComponent>(skcs);
	for (USkeletalMeshComponent* m : skcs)
	{
		if (m)
		{
			m->SetRenderCustomDepth(value);
		}
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

