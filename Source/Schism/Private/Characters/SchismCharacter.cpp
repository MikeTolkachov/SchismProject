
#include "Characters/SchismCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/SchismHUD.h"
#include "HUD/SchismOverlay.h"

ASchismCharacter::ASchismCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");


}

void ASchismCharacter::Tick(float DeltaTime)
{
	if (Attributes && SchismOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		SchismOverlay->SetStaminaProgerssBarPercent(Attributes->GetStaminaPercent());
	}
}

void ASchismCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASchismCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASchismCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASchismCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASchismCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASchismCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASchismCharacter::Dodge);
	}

}

void ASchismCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		Super::Jump();
	}
}

void ASchismCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Attributes && Attributes->GetHealthPercent() > 0)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ASchismCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ASchismCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && SchismOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		SchismOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ASchismCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && SchismOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SchismOverlay->SetGold(Attributes->GetGold());
	}
}

float ASchismCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASchismCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(SchismContext, 0);
		}
	}
	Tags.Add(FName("EngageableTarget"));
	InitializeSchismOverlay(PlayerController);
}

void ASchismCharacter::Move(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void ASchismCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASchismCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else 
	{	
		if (CanDisarm()) Disarm();
		else if (CanArm()) Arm();
	}
}

void ASchismCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;

	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && SchismOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SchismOverlay->SetStaminaProgerssBarPercent(Attributes->GetStaminaPercent());
	}
}

void ASchismCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquipeedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void ASchismCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASchismCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASchismCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASchismCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASchismCharacter::CanAttack()
{
	return 	ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void ASchismCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
}

bool ASchismCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASchismCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		   CharacterState == ECharacterState::ECS_Unequipped &&
		   EquippedWeapon;
}

bool ASchismCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool ASchismCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void ASchismCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASchismCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquipeedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASchismCharacter::AttachWeaponToSpine()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASchismCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASchismCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASchismCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASchismCharacter::InitializeSchismOverlay(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (ASchismHUD* SchismHUD = Cast<ASchismHUD>(PlayerController->GetHUD()))
		{
			SchismOverlay = SchismHUD->GetSchismOverlay();
			if (SchismOverlay && Attributes)
			{
				SchismOverlay->SetHealthProgerssBarPercent(Attributes->GetHealthPercent());
				SchismOverlay->SetStaminaProgerssBarPercent(1.f);
				SchismOverlay->SetGold(0);
				SchismOverlay->SetSouls(0);
			}
		}
	}

}

void ASchismCharacter::SetHUDHealth()
{
	if (SchismOverlay && Attributes)
	{
		SchismOverlay->SetHealthProgerssBarPercent(Attributes->GetHealthPercent());
	}
}

