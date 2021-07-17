// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CGBPUtils.generated.h"

class UDataTable;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class CHESS_API UCGBPUtils : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	static void FillCBFromDT(UDataTable* DataTable, UComboBoxString* ComboBox);
};
