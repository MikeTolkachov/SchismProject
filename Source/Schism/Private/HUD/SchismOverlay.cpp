// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SchismOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USchismOverlay::SetHealthProgerssBarPercent(float Percent)
{
	if (HealthProgerssBar)
	{
		HealthProgerssBar->SetPercent(Percent);
	}
}

void USchismOverlay::SetStaminaProgerssBarPercent(float Percent)
{
	if (StaminaProgerssBar)
	{
		StaminaProgerssBar->SetPercent(Percent);
	}
}

void USchismOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String);
		GoldText->SetText(Text);
	}
}

void USchismOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FString String = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(String);
		SoulsText->SetText(Text);
	}
}
