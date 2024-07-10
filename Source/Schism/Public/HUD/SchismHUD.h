// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SchismHUD.generated.h"

class USchismOverlay;

UCLASS()
class SCHISM_API ASchismHUD : public AHUD
{
	GENERATED_BODY()
protected: 
	virtual void BeginPlay() override;
private: 
	UPROPERTY(EditDefaultsOnly, Category = "Schism")
	TSubclassOf<USchismOverlay> SchismOverlayClass;

	UPROPERTY()
	USchismOverlay* SchismOverlay;
public: 
	FORCEINLINE USchismOverlay* GetSchismOverlay() const { return SchismOverlay; }
};
