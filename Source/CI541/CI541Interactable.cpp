// Copyright (c) 2020 Jack C. Lloyd. All rights reserved.

#include "CI541Interactable.h"
#include "CI541Inventory.h"
#include "CI541Player.h"

// Copyright (c) 1998-2019 Epic Games, Inc. All rights reserved.

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"

FName ACI541Interactable::StaticMeshComponentName(TEXT("StaticMesh"));
FName ACI541Interactable::SphereComponentName(TEXT("Sphere"));
FName ACI541Interactable::SoundBaseComponentName(TEXT("SoundBase"));

ACI541Interactable::ACI541Interactable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(ACI541Interactable::SphereComponentName);

	if (IsValid(Sphere))
	{
		Sphere->InitSphereRadius(50);
		Sphere->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);

		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACI541Interactable::OnBeginOverlap);

		RootComponent = Sphere;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"Sphere\""));
	}

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(ACI541Interactable::StaticMeshComponentName);

	if (IsValid(StaticMesh))
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		StaticMesh->SetupAttachment(Sphere);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>
		StaticMeshFinder(TEXT("StaticMesh'/Game/Weapons/Meshes/SM_AssaultRifle_Ammunition.SM_AssaultRifle_Ammunition'"));

		if (StaticMeshFinder.Succeeded())
		{
			StaticMesh->SetStaticMesh(StaticMeshFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *StaticMeshFinder.GetReferencerName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"StaticMesh\""));
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
	SoundBaseFinder(TEXT("SoundCue'/Game/Weapons/Sound/Rifle_AmmoPickup_Cue.Rifle_AmmoPickup_Cue'"));

	if (SoundBaseFinder.Succeeded())
	{
		SoundBase = SoundBaseFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *SoundBaseFinder.GetReferencerName());
	}
}

//~ Begin USphereComponent Interface.
void ACI541Interactable::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex,
                                        bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && (OtherActor != this) && IsValid(OtherComp))
	{
		ACI541Player* Player = Cast<ACI541Player>(OtherActor);

		if (IsValid(Player))
		{
			if (Player->GetInventory()->Add(Item))
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundBase, GetActorLocation());
				Destroy();
			}
		}
	}
}
//~ End USphereComponent Interface.