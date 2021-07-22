// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CGLabelWidgetComponent.h"
#include "UI/CGLabelWidget.h"

void UCGLabelWidgetComponent::SetLabel(int iPacked)
{
	if (UCGLabelWidget* label = Cast<UCGLabelWidget>(GetWidget()))
	{
		label->SetBoardLabel(iPacked);
	}
}