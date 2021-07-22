// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGHighlightableComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHESS_API UCGHighlightableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCGHighlightableComponent();

	//UPROPERTY(EditAnywhere, Category = "Chess setup")
	//bool bIsSkeletal;

	UFUNCTION(BlueprintCallable, Category="Chess setup")
	virtual void SetHighlighted(bool value);
};
