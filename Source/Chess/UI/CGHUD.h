// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "CGHUD.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ACGHUD : public AHUD
{
	GENERATED_BODY()
private:
	UUserWidget* m_MainMenu;
	UUserWidget* m_OptionsMenu;
	UUserWidget* m_InGameMenu;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> MainMenu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> OptionsMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> InGameMenu;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Chess HUD")
	void ShowOptions();

};

