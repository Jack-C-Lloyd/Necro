// Copyright (c) 2019-2020 Jack C. Lloyd. All rights reserved.

#include "CI541Weapon.h"

#include "CI541Ammunition.h"
#include "CI541Inventory.h"
#include "CI541Item.h"
#include "CI541Projectile.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

FName ACI541Weapon::SkeletalMeshComponentName(TEXT("SkeletalMeshComponent"));
FName ACI541Weapon::ParticleSystemComponentName(TEXT("ParticleSystemComponent"));
FName ACI541Weapon::ParticleSystemSocketName(TEXT("MuzzleFlash"));

ACI541Weapon::ACI541Weapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(ACI541Weapon::SkeletalMeshComponentName);

	if (IsValid(SkeletalMesh))
	{
		SkeletalMesh->AlwaysLoadOnClient = true;
		SkeletalMesh->AlwaysLoadOnServer = true;
		SkeletalMesh->bOwnerNoSee = false;
		SkeletalMesh->bCastDynamicShadow = true;
		SkeletalMesh->bAffectDynamicIndirectLighting = true;

		SkeletalMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		SkeletalMesh->SetGenerateOverlapEvents(false);
		SkeletalMesh->SetCanEverAffectNavigation(false);

		RootComponent = SkeletalMesh;

		static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMeshFinder(TEXT("SkeletalMesh'/Game/Weapons/Meshes/SK_AssaultRifle_SkeletalMesh.SK_AssaultRifle_SkeletalMesh'"));

		if (SkeletalMeshFinder.Succeeded())
		{
			SkeletalMesh->SetSkeletalMesh(SkeletalMeshFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *SkeletalMeshFinder.GetReferencerName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"Mesh\""));
	}

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(ACI541Weapon::ParticleSystemComponentName);

	if (IsValid(ParticleSystem))
	{
		ParticleSystem->bAutoActivate = false;

		ParticleSystem->SetupAttachment(SkeletalMesh, ACI541Weapon::ParticleSystemSocketName);

		static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystemFinder(TEXT("ParticleSystem'/Game/Weapons/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));

		if (ParticleSystemFinder.Succeeded())
		{
			ParticleSystem->SetTemplate(ParticleSystemFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *ParticleSystemFinder.GetReferencerName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"ParticleSystem\""));
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
	SoundBaseFinder(TEXT("SoundCue'/Game/Weapons/Sound/RifleA_Fire_Cue.RifleA_Fire_Cue'"));

	if (SoundBaseFinder.Succeeded())
	{
		SoundBase = SoundBaseFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *SoundBaseFinder.GetReferencerName());
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	AnimMontageFinder(TEXT("AnimMontage'/Game/Characters/Player/Animations/AM_Reload_AssaultRifle.AM_Reload_AssaultRifle'"));

	if (AnimMontageFinder.Succeeded())
	{
		AnimMontage = AnimMontageFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *AnimMontageFinder.GetReferencerName());
	}

	Ammunition = UCI541Ammunition::StaticClass();
	Projectile = ACI541Projectile::StaticClass();
}

void ACI541Weapon::BeginPlay()
{
	Loaded = Capacity;
}

bool ACI541Weapon::Fire()
{
	if (Loaded > 0)
	{
		OnFire();

		Loaded--;

		ParticleSystem->Activate(true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundBase, GetActorLocation());

		FActorSpawnParameters ProjectileSpawnParameters;
		ProjectileSpawnParameters.Owner = this;

		GetWorld()->SpawnActor<ACI541Projectile>(Projectile,
			SkeletalMesh->GetSocketLocation(ACI541Weapon::ParticleSystemSocketName),
			SkeletalMesh->GetSocketRotation(ACI541Weapon::ParticleSystemSocketName),
			ProjectileSpawnParameters);

		return true;
	}

	return false;
}

bool ACI541Weapon::Reload(UCI541Inventory* InInventory)
{
	bool bReloaded = false;

	if (IsValid(InInventory))
	{
		UCI541Item* Item = InInventory->Contains(Ammunition.Get());

		if (IsValid(Item))
		{
			while (!Item->IsEmpty() && (Loaded != Capacity))
			{
				bReloaded = true;
				OnReload();
				Item->Decrement();
				Loaded++;
			}

			if (Item->IsEmpty())
			{
				InInventory->Remove(Item);
			}
		}
	}

  return bReloaded;
}