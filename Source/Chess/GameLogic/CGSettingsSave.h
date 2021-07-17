// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FString PlayerName {""};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	int SelectedSkin{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	int PreferredSide{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	int Player2SelectedSkin{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess settings")
	int Player2PreferredSide{ 0 };
};
