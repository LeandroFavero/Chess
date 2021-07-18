// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameLogic/CGSkinData.h"
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "ChessLogic/CGChessBoard.h"
#include "GameFramework/GameMode.h"
#include "CGGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CHESS_API ACGGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACGGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	bool bHotSeatMode;

	virtual void BeginPlay() override;

	void GenericPlayerInitialization(AController* Controller) override;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	int GetCurrentViewMode(const APlayerController* PlayerController);
};
