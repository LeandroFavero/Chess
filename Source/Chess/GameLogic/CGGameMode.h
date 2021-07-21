// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic/CGSide.h"
#include "GameFramework/GameMode.h"
#include "CGGameMode.generated.h"


class APlayerController;
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

	//virtual void BeginPlay() override;

	void StartMatch() override;
	void PostLogin(APlayerController* NewPlayer) override;

	void HandleMatchHasStarted() override;

};
