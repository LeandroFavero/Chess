// Fill out your copyright notice in the Description page of Project Settings.

#include "CGHUD.h"

#define Dbg(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

void ACGHUD::BeginPlay()
{
	if (MainMenu)
	{
		m_MainMenu = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenu);
		if (m_MainMenu) 
		{
			m_MainMenu->AddToViewport();
		}
	}
	if (OptionsMenu)
	{
		m_OptionsMenu = CreateWidget<UUserWidget>(GetOwningPlayerController(), OptionsMenu);
	}
	if (InGameMenu)
	{
		m_InGameMenu = CreateWidget<UUserWidget>(GetOwningPlayerController(), InGameMenu);
	}
}

void ACGHUD::ShowOptions()
{
	if (m_OptionsMenu)
	{
		m_OptionsMenu->AddToViewport();
	}
}