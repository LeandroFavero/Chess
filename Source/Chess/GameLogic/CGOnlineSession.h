// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineSession.h"
#include "CGOnlineSession.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UCGOnlineSession : public UOnlineSession
{
	GENERATED_BODY()
	

	void HandleDisconnect(UWorld* World, class UNetDriver* NetDriver) override;
};
