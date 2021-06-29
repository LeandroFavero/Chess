// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameLogic/CGSkinData.h"
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "ChessLogic/CGChessBoard.h"
#include "GameFramework/GameModeBase.h"
#include "ChessGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChessGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AChessGameMode();


	UPROPERTY(EditAnywhere, Category = "Chess setup")
	UDataTable* Skins;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	ACGChessBoard* Board;

	//UFUNCTION(BlueprintPure, Category = "Chess setup")
	//FCGSkinData* GetSkinData(FName Id);
	UFUNCTION(BlueprintCallable, Category = "Debug")
	int GetCurrentViewMode(const APlayerController* PlayerController);
};
