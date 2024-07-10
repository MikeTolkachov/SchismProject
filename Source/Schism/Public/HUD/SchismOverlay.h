// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SchismOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SCHISM_API USchismOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthProgerssBarPercent(float Percent);
	void SetStaminaProgerssBarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSouls(int32 Souls);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgerssBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgerssBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulsText;

};
