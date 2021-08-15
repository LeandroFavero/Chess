// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UCIEngineHandler.generated.h"

class ACGChessBoard;
class ACGChessPlayerController;
class FInteractiveProcess;

UENUM(BlueprintType)
enum EUCIState
{
	NOT_RUNNING,
	READY,
	THINKING,

};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Config = UCISettings)
class CHESS_API UUCIEngineHandler : public UObject
{
	GENERATED_BODY()

	float TurnTime;
	int Elo;
	FString EnginePath;
	ACGChessPlayerController* Controller;
	ACGChessBoard* Board;
	TSharedPtr<FInteractiveProcess> EngineProc;
	FTimerHandle TimerHandle;

public:
	UPROPERTY(BlueprintReadOnly, Category = "UCI")
	TEnumAsByte<EUCIState> State;

	UUCIEngineHandler();
	UFUNCTION(BlueprintCallable, Category = "UCI")
	void StartEngine(const FString iEnginePath, const int iTurnTime, const int iElo, ACGChessPlayerController* iController, ACGChessBoard* iBoard);
	UFUNCTION(BlueprintCallable, Category = "UCI")
	void ApplySettings();
	UFUNCTION(BlueprintCallable, Category = "UCI")
	void StopEngine();
	UFUNCTION(BlueprintCallable, Category = "UCI")
	void GetNextMove();
	void BeginDestroy() override;
	UFUNCTION(BlueprintCallable, Category = "UCI")
	bool IsReady();

	UFUNCTION(BlueprintCallable, Category = "UCI")
	void SendCommand(const FString& iCmd);
private:
	void OnTurnTimeUp();
	UFUNCTION()
	void OnReceive(const FString& iReceived);
	UFUNCTION()
	void OnStopped();
	FString SearchForExe() { return ""; }
};