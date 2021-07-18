// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/CGBPUtils.h"
#include "Engine/DataTable.h"
#include "Components/ComboBoxString.h"
#include "GameLogic/CGSkinData.h"
#include "GameLogic/CGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void UCGBPUtils::FillCBFromDT(UDataTable* DataTable, UComboBoxString* ComboBox)
{
	if (!DataTable || !ComboBox)
	{
		return;
	}

	ComboBox->ClearOptions();

	for (const auto& it : DataTable->GetRowMap())
	{
		FCGDataBase* row = (FCGDataBase*)(it.Value);
		if (row)
		{
			ComboBox->AddOption(row->Name.ToString());
		}
	}
}

bool UCGBPUtils::IsHotSeatMode(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		if (ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>())
		{
			FString m = UGameplayStatics::ParseOption(mode->OptionsString, "Mode");
			if (m.Equals("HS", ESearchCase::IgnoreCase))
			{
				return true;
			}
		}
	}
	return false;
}

const FString UCGBPUtils::FenFromMapParameter(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		if (ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>())
		{
			return UGameplayStatics::ParseOption(mode->OptionsString, "Fen");
		}
	}
	return FString();
}

bool UCGBPUtils::IsWaitingForPlayers(const UObject* WorldContextObject)
{
	if (IsHotSeatMode(WorldContextObject))
	{
		return false;
	}
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return w->GetNumPlayerControllers() != 2;
	}
	return false;
}

bool UCGBPUtils::IsGameConcluded(const UObject* WorldContextObject)
{
	return false;
}

bool UCGBPUtils::IsClient(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return GEngine->GetNetMode(w) == NM_Client;
	}
	return false;
}

bool UCGBPUtils::IsListenServer(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return GEngine->GetNetMode(w) == NM_ListenServer;
	}
	return false;
}

bool UCGBPUtils::IsStandalone(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return GEngine->GetNetMode(w) == NM_Standalone;
	}
	return false;
}
