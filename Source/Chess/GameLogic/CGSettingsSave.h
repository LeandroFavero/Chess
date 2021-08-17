// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic/CGSide.h"
#include "GameFramework/SaveGame.h"
#include "CGSettingsSave.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UCGSettingsSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	FText PlayerName = FText::FromString(TEXT(""));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	FString SelectedSkin = TEXT("Default");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	TEnumAsByte<ESide> PreferredSide = ESide::WHITE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	FString LastIp = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	FString EnginePath = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	int EngineElo = 1200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	int EngineThinkTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	bool bIsLan = true;
};
