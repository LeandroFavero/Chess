// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/CGAnimInstance.h"
#include "Net/UnrealNetwork.h"

void UCGAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCGAnimInstance, bIsGrabbed)
}