// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SchismHUD.h"
#include "HUD/SchismOverlay.h"

void ASchismHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && SchismOverlayClass)
		{
			SchismOverlay = CreateWidget<USchismOverlay>(PlayerController, SchismOverlayClass);
			SchismOverlay->AddToViewport();
		}
	}

}
