// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGBPUtils.generated.h"

class UDataTable;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class CHESS_API UCGBPUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	static void FillCBFromDT(UDataTable* DataTable, UComboBoxString* ComboBox);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Chess utils")
	static bool IsHotSeatMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Chess utils")
	static const FString FenFromMapParameter(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Chess utils")
	static bool IsWaitingForPlayers(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Chess utils")
	static bool IsGameConcluded(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Chess utils")
	static bool IsClient(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Chess utils")
	static bool IsListenServer(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Chess utils")
	static bool IsStandalone(const UObject* WorldContextObject);

};
