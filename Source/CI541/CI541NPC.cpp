// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#include "CI541NPC.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

ACI541NPC::ACI541NPC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	USkeletalMeshComponent* SkeletalMesh = GetMesh();

	if (IsValid(SkeletalMesh))
	{
		float NegativeHalfHeight = -GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		float ForwardAngleOffset = -90.0f;

		SkeletalMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, NegativeHalfHeight), FRotator(0.0f, ForwardAngleOffset, 0.0f));

		//SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		//SkeletalMesh->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

		static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMeshFinder(TEXT("SkeletalMesh'/Game/Characters/NPC/Mesh/SK_NPC_SkeletalMesh.SK_NPC_SkeletalMesh'"));

		if (SkeletalMeshFinder.Succeeded())
		{
			SkeletalMesh->SetSkeletalMesh(SkeletalMeshFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *SkeletalMeshFinder.GetReferencerName());
		}

		// static ConstructorHelpers::FObjectFinder<UAnimBlueprint>
		// AnimBlueprintFinder(TEXT("AnimBlueprint'/Game/Characters/NPC/Blueprints/BP_NPC_AnimBlueprint.BP_NPC_AnimBlueprint'"));

		static ConstructorHelpers::FObjectFinder<UClass>
		AnimBlueprintFinder(TEXT("AnimBlueprint'/Game/Characters/NPC/Blueprints/BP_NPC_AnimBlueprint.BP_NPC_AnimBlueprint_C'"));

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

	UCapsuleComponent* CapsuleComponent= GetCapsuleComponent();

	if (IsValid(CapsuleComponent))
	{
		CapsuleComponent->SetCapsuleHalfHeight(88.0f);
		CapsuleComponent->SetCapsuleRadius(34.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to access default subobject \"CapsuleComponent\""));
	}

	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();

	if (IsValid(CharacterMovement))
	{
		CharacterMovement->CrouchedHalfHeight = 68.0f;
		CharacterMovement->MaxWalkSpeed = 225.0f;
		CharacterMovement->MaxWalkSpeedCrouched = 180.0f;
		CharacterMovement->bCanWalkOffLedges = false;
		CharacterMovement->bCanWalkOffLedgesWhenCrouching = false;
		CharacterMovement->MaxCustomMovementSpeed = 180.0f;
		CharacterMovement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
		CharacterMovement->bUseControllerDesiredRotation = true;
		CharacterMovement->NavAgentProps.bCanCrouch = true;
		CharacterMovement->NavAgentProps.bCanJump = false;
		CharacterMovement->NavAgentProps.bCanWalk = true;
		CharacterMovement->NavAgentProps.bCanSwim = false;
		CharacterMovement->NavAgentProps.bCanFly = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to access default subobject \"CharacterMovement\""));
	}
}

void ACI541NPC::Damage(float InDamage)
{
	if ((Health -= InDamage) < 0.01f)
	{
		OnDeath();
	}
}