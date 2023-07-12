// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#include "CI541Player.h"
#include "CI541Inventory.h"
#include "CI541Item.h"
#include "CI541Weapon.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

FName ACI541Player::SpringArmComponentName(TEXT("SpringArm"));
FName ACI541Player::SpringArmSocketName(TEXT("head_socket"));
FName ACI541Player::CameraComponentName(TEXT("Camera"));
FName ACI541Player::InventoryComponentName(TEXT("Inventory"));
FName ACI541Player::ChildActorComponentName(TEXT("ChildActor"));
FName ACI541Player::ChildActorSocketName(TEXT("hand_r_socket"));

ACI541Player::ACI541Player(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	USkeletalMeshComponent* SkeletalMesh = GetMesh();

	if (IsValid(SkeletalMesh))
	{
		float NegativeHalfHeight = -GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		float ForwardAngleOffset = -90.0f;

		SkeletalMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, NegativeHalfHeight), FRotator(0.0f, ForwardAngleOffset, 0.0f));

		// SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		// SkeletalMesh->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

		static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMeshFinder(TEXT("SkeletalMesh'/Game/Characters/Player/Mesh/SK_Player_SkeletalMesh.SK_Player_SkeletalMesh'"));

		if (SkeletalMeshFinder.Succeeded())
		{
			SkeletalMesh->SetSkeletalMesh(SkeletalMeshFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *SkeletalMeshFinder.GetReferencerName());
		}

		// static ConstructorHelpers::FObjectFinder<UAnimBlueprint>
		// AnimBlueprintFinder(TEXT("AnimBlueprint'/Game/Characters/Player/Blueprints/BP_Player_AnimBlueprint.BP_Player_AnimBlueprint'"));

		static ConstructorHelpers::FObjectFinder<UClass>
		AnimBlueprintFinder(TEXT("AnimBlueprint'/Game/Characters/Player/Blueprints/BP_Player_AnimBlueprint.BP_Player_AnimBlueprint_C'"));

		// if (AnimBlueprintFinder.Succeeded())
		if (AnimBlueprintFinder.Object != NULL)
		{
			// SkeletalMesh->SetAnimInstanceClass(AnimBlueprintFinder.Object->GetAnimBlueprintGeneratedClass());
			SkeletalMesh->SetAnimInstanceClass(AnimBlueprintFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *AnimBlueprintFinder.GetReferencerName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to access default subobject \"SkeletalMesh\""));
	}

	UCapsuleComponent* _CapsuleComponent = GetCapsuleComponent();

	if (IsValid(_CapsuleComponent))
	{
		_CapsuleComponent->SetCapsuleHalfHeight(88.0f);
		_CapsuleComponent->SetCapsuleRadius(34.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to access default subobject \"CapsuleComponent\""));
	}

	UCharacterMovementComponent* _CharacterMovement = GetCharacterMovement();

	if (IsValid(_CharacterMovement))
	{
		_CharacterMovement->CrouchedHalfHeight = 68.0f;
		_CharacterMovement->MaxWalkSpeed = 270.0f;
		_CharacterMovement->MaxWalkSpeedCrouched = 180.0f;
		_CharacterMovement->bCanWalkOffLedges = true;
		_CharacterMovement->bCanWalkOffLedgesWhenCrouching = true;
		_CharacterMovement->MaxCustomMovementSpeed = 180.0f;
		_CharacterMovement->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
		_CharacterMovement->bUseControllerDesiredRotation = true;
		_CharacterMovement->NavAgentProps.bCanCrouch = true;
		_CharacterMovement->NavAgentProps.bCanJump = false;
		_CharacterMovement->NavAgentProps.bCanWalk = true;
		_CharacterMovement->NavAgentProps.bCanSwim = false;
		_CharacterMovement->NavAgentProps.bCanFly = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to access default subobject \"CharacterMovement\""));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(ACI541Player::SpringArmComponentName);

	if (IsValid(SpringArm))
	{
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
		
		SpringArm->SetupAttachment(GetMesh(), ACI541Player::SpringArmSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"SpringArm\""));
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(ACI541Player::CameraComponentName);

	if (IsValid(Camera))
	{
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"Camera\""));
	}

	Inventory = CreateDefaultSubobject<UCI541Inventory>(ACI541Player::InventoryComponentName);

	if (!IsValid(Inventory))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"Inventory\""));
	}

	ChildActor = CreateDefaultSubobject<UChildActorComponent>(ACI541Player::ChildActorComponentName);

	if (IsValid(ChildActor))
	{
		ChildActor->SetRelativeLocationAndRotation(FVector(-7.0f, 6.0f, -3.0f), FRotator(0.0f, 90.0f, 0.0f));

		// ChildActor->RelativeLocation = FVector(-7.0f, 6.0f, -3.0f);
		// ChildActor->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);

		ChildActor->SetupAttachment(GetMesh(), ACI541Player::ChildActorSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"ChildActor\""));
	}

	bIsAiming = false;
	bIsCrouching = false;
	bIsReloading = false;
	bView = false;

	Health = 1.0f;
	bIsInfected = false;
}

void ACI541Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInfected)
	{
		Damage(DeltaTime * InfectionRate);
	}
}

void ACI541Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//~ Begin UInputComponent::ActionBindings Interface.
	PlayerInputComponent->BindAction("Aim",    IE_Pressed,  this, &ACI541Player::Aim);
	PlayerInputComponent->BindAction("Aim",    IE_Released, this, &ACI541Player::UnAim);
	PlayerInputComponent->BindAction("Fire",   IE_Pressed,  this, &ACI541Player::Fire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed,  this, &ACI541Player::Reload);
	PlayerInputComponent->BindAction("View",   IE_Pressed,  this, &ACI541Player::ToggleView);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed,  this, &ACI541Player::ToggleCrouch);
	//~ End UInputComponent::ActionBindings Interface.

	//~ Begin UInputComponent::AxisBindings Interface.
	PlayerInputComponent->BindAxis("Turn",        this, &ACI541Player::Turn);
	PlayerInputComponent->BindAxis("LookUp",      this, &ACI541Player::LookUp);
	PlayerInputComponent->BindAxis("MoveRight",   this, &ACI541Player::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACI541Player::MoveForward);
	//~ End UInputComponent::AxisBindings Interface.
}

//~ Begin UInputComponent::ActionBindings Interface.
void ACI541Player::Aim()
{
	bIsAiming = true;
}

void ACI541Player::UnAim()
{
	bIsAiming = false;
}

void ACI541Player::Fire()
{
	if (bIsReloading)
	{
		return;
	}

	ACI541Weapon* Weapon = Cast<ACI541Weapon>(ChildActor->GetChildActor());

	if (IsValid(Weapon))
	{
		if (!Weapon->Fire())
		{
			Reload();
		};
	}
}

void ACI541Player::Reload()
{
	if (bIsReloading)
	{
		return;
	}

	if (IsValid(ChildActor))
	{
		ACI541Weapon* Weapon = Cast<ACI541Weapon>(ChildActor->GetChildActor());

		if (IsValid(Weapon) && Weapon->Reload(Inventory))
		{
			bIsReloading = true;

			UAnimMontage* AnimMontage = Weapon->GetAnimMontage();

			if (IsValid(AnimMontage))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);

				FOnMontageEnded OnMontageEndedDelegate;
				OnMontageEndedDelegate.BindUObject(this, &ACI541Player::Reloaded);
					
				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEndedDelegate);
			}
		}
	}
}

void ACI541Player::Reloaded(UAnimMontage *InAnimMontage, bool InBool)
{
	bIsReloading = false;
}

void ACI541Player::ToggleView()
{
	if ((bView = !bView) != 0)
	{
		Camera->SetRelativeRotation(FRotator(0.0f, 5.0f, 0.0f));

		SpringArm->TargetArmLength = 0;
		SpringArm->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
		SpringArm->bEnableCameraLag = false;
		SpringArm->bEnableCameraRotationLag = false;
	}
	else
	{
		Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		SpringArm->TargetArmLength = 100;
		SpringArm->SocketOffset = FVector(0.0f, 50.0f, 0.0f);
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
	}
}

void ACI541Player::ToggleCrouch()
{
	((bIsCrouching = !bIsCrouching) != 0) ? Crouch() : UnCrouch();
}
//~ End UInputComponent::ActionBindings Interface.

//~ Begin UInputComponent::AxisBindings Interface.
void ACI541Player::Turn(float InAxis)
{
	AddControllerYawInput(InAxis);
}

void ACI541Player::LookUp(float InAxis)
{
	AddControllerPitchInput(InAxis);
}

void ACI541Player::MoveRight(float InAxis)
{
	AddMovementInput(GetActorRightVector(), InAxis);
}

void ACI541Player::MoveForward(float InAxis)
{
	AddMovementInput(GetActorForwardVector(), InAxis);
}
//~ End UInputComponent::AxisBindings Interface.

void ACI541Player::Damage(float InDamage)
{
	Health = ((Health -= InDamage) >= 0.0f) ? Health : 0.0f;

	if (Health == 0.0f)
	{
		Die();
	}
}

void ACI541Player::Heal(float InHealth)
{
	Health = ((Health += InHealth) <= 1.0f) ? Health : 1.0f;
}

void ACI541Player::Die()
{
	OnDie();
}

void ACI541Player::Infect()
{
	bIsInfected = true;
}

void ACI541Player::Cure()
{
	bIsInfected = false;
}

//~ Begin UCI541Item Interface.
void ACI541Player::OnUse(UCI541Item* InItem)
{
	if (IsValid(InItem))
	{
		InItem->Use(this);   // C++.
		InItem->OnUse(this); // Blueprints.
	}
}
//~ End UCI541Item Interface.