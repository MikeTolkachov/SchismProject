
#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;
class AAIController;

UCLASS()
class SCHISM_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public: 
	AEnemy();

	/* <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/* </AActor> */

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* <IHitInterface> */
protected: 
	virtual void BeginPlay() override;
	void SpawnSoul();

	/* <ABaseCharacter> */
	virtual void Die_Implementation() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	/* </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private: 

	// AI Behavior
	void InitializeEnemy();
	void SpawnDefaultWeapon();

	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void ClearPatrolTimer();
	void StartPatrolling();
	AActor* ChoosePatrolTarget();

	void HideHealthBar();
	void ShowHealthBar();

	void LoseInterest();
	void MoveToTarget(AActor* Target);
	void ChaseTarget();

	void StartAttackTimer();
	void ClearAttackTimer();

	bool InTargetRange(AActor* Target, double Radius);
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsEngaged();
	bool IsDead();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); //Callback for OnPawnSeen in UPawnSensingComponent


	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	double CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	double AttackRadius = 150.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	double AcceptanceRadius = 50.f;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 10.f;
		
	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200.f;	
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;
};
