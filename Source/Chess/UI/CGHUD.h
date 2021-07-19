// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GameLogic/CGUndo.h"
#include "CGHUD.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ACGHUD : public AHUD
{
	GENERATED_BODY()

	UUserWidget* CurrentWidget;
	void InitHUD();

public:
	UUserWidget* MainMenu;
	UUserWidget* InGame;

	ACGHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> MainMenuTemplate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> InGameTemplate;

	virtual void ShowMenu();
	virtual void ShowGame();
};

