// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CGSkinData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CHESS_API FCGSkinData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInstance* WhiteMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInstance* BlackMaterial;
};
