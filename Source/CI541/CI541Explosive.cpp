// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#include "CI541Explosive.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

FName ACI541Explosive::ParticleSystemComponentName(TEXT("ParticleSystemComponent"));
FName ACI541Explosive::SoundBaseComponentName(TEXT("SoundBaseComponent"));

ACI541Explosive::ACI541Explosive(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UStaticMeshComponent* _StaticMesh = GetStaticMesh();

	if (IsValid(_StaticMesh))
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshFinder(TEXT("StaticMesh'/Game/Weapons/Meshes/SM_GrenadeLauncher_Projectile.SM_GrenadeLauncher_Projectile'"));

		if (MeshFinder.Succeeded())
		{
			_StaticMesh->SetStaticMesh(MeshFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *MeshFinder.GetReferencerName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to access default subobject \"Mesh\""));
	}

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(ACI541Explosive::ParticleSystemComponentName);

	if (IsValid(ParticleSystem))
	{
		ParticleSystem->bAutoActivate = false;

		static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ParticleSystemFinder(TEXT("ParticleSystem'/Game/Weapons/FX/P_GrenadeLauncher_Explosion.P_GrenadeLauncher_Explosion'"));

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
	SoundBaseFinder(TEXT("SoundCue'/Game/Weapons/Sound/GL_Explosion_Cue.GL_Explosion_Cue'"));

	if (SoundBaseFinder.Succeeded())
	{
		SoundBase = SoundBaseFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *SoundBaseFinder.GetReferencerName());
	}
}

bool ACI541Explosive::Expire()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem->Template, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundBase, GetActorLocation());

	return Super::Expire();
}
