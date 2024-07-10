// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class SCHISM_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	//Combat
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack();
	
	UFUNCTION(BlueprintNativeEvent)
	void Die();
	
	virtual void HandleDamage(float DamageAmount);
	virtual bool CanAttack();
	void DirectionalHitReact(const FVector& ImpactPoint);
	bool IsAlive();
	void DisableCapsule();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableMeshCollision();

	//Montage
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual void PlayDodgeMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;

private:

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UAnimMontage* DodgeMontage;
public: 
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
