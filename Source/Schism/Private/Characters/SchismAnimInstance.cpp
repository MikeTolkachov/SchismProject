// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SchismAnimInstance.h"
#include "Characters/SchismCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USchismAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SchismCharacter = Cast<ASchismCharacter>(TryGetPawnOwner());
	if (SchismCharacter)
	{
		SchismCharacterMovement = SchismCharacter->GetCharacterMovement();
	}
}

void USchismAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SchismCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SchismCharacterMovement->Velocity);
		IsFalling = SchismCharacterMovement->IsFalling();
		CharacterState = SchismCharacter->GetChatacterState();
		ActionState = SchismCharacter->GetActionState();
		DeathPose = SchismCharacter->GetDeathPose();
	}
}
