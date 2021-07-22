// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "CGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UCGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess Anim")
	bool bIsGrabbed { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess Anim")
	float Speed { 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess Anim")
	float Scale { 1.0f };

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
