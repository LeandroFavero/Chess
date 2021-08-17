// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGLabelWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UCGLabelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Chess setup")
	int DigitMask = 1 << 31;

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess setup")
	void SetBoardLabel(int Packed);
};
