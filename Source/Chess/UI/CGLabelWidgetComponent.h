// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CGLabelWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UCGLabelWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="Chess setup")
	void SetLabel(int Packed);
};
