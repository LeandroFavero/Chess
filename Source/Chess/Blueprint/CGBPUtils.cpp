// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/CGBPUtils.h"
#include "Engine/DataTable.h"
#include "Components/ComboBoxString.h"
#include "GameLogic/CGSkinData.h"


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