// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SchismCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;
class USchismOverlay;

UCLASS()
class SCHISM_API ASchismCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ASchismCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* <IHitInterface> */

	/* <UPickupInterface> */
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	/* <UPickupInterface> */

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;

	//Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Dodge();



	//Combat
	void EquipWeapon(AWeapon* Weapon);
	void PlayEquipMontage(const FName& SectionName);
	/* <ABaseCharacter> */
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	virtual void Die_Implementation() override;
	/* </ABaseCharacter> */

	bool CanDisarm();
	bool CanArm();
	bool HasEnoughStamina();
	bool IsOccupied();	
	void Disarm();
	void Arm();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToSpine();
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UPROPERTY(EditAnywhere, Category = Input);
	UInputMappingContext* SchismContext;
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* EKeyAction;
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* DodgeAction;
private: 
	void InitializeSchismOverlay(APlayerController* PlayerController);
	void SetHUDHealth();

	//Character Components
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EyeBrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	//States
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	USchismOverlay* SchismOverlay;


public: 
	FORCEINLINE ECharacterState GetChatacterState() const {return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
