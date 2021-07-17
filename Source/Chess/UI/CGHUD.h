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

public:
	UUserWidget* MainMenu;
	UUserWidget* OptionsMenu;
	UUserWidget* InGame;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> MainMenuTemplate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> OptionsMenuTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> InGameTemplate;

	virtual void ShowHud();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess")
	void UpdateHud();

	UFUNCTION(BlueprintCallable, Category = "Chess HUD")
	FString UndoToNotationString(const FCGUndo& undo) const;

};

