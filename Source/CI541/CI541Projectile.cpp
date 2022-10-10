// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#include "CI541Projectile.h"
#include "CI541NPC.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"

FName ACI541Projectile::StaticMeshComponentName(TEXT("StaticMeshComponent"));
FName ACI541Projectile::BoxComponentName(TEXT("BoxComponent"));
FName ACI541Projectile::ProjectileMovementComponentName(TEXT("ProjectileMovementComponent"));

ACI541Projectile::ACI541Projectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	Box = CreateDefaultSubobject<UBoxComponent>(ACI541Projectile::BoxComponentName);
	
	if (IsValid(Box))
	{
		Box->bDynamicObstacle = true;

		Box->InitBoxExtent(FVector(10.0f, 10.0f, 10.0f));
		Box->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
		Box->SetShouldUpdatePhysicsVolume(true);
		Box->SetCanEverAffectNavigation(false);

		Box->OnComponentHit.AddDynamic(this, &ACI541Projectile::OnHit);

		RootComponent = Box;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"Box\""));
	}
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(ACI541Projectile::ProjectileMovementComponentName);

	if (IsValid(ProjectileMovement))
	{
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bInitialVelocityInLocalSpace = true;
		ProjectileMovement->bShouldBounce = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create default subobject \"ProjectileMovement\""));
	}

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(ACI541Projectile::StaticMeshComponentName);
	
	if (IsValid(StaticMesh))
	{
		StaticMesh->AlwaysLoadOnClient = true;
		StaticMesh->AlwaysLoadOnServer = true;
		StaticMesh->bOwnerNoSee = false;
		StaticMesh->bCastDynamicShadow = true;
		StaticMesh->bAffectDynamicIndirectLighting = true;

		StaticMesh->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
		StaticMesh->SetGenerateOverlapEvents(false);
		StaticMesh->SetCanEverAffectNavigation(false);
		StaticMesh->SetupAttachment(Box);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>
		StaticMeshFinder(TEXT("StaticMesh'/Game/Weapons/Meshes/SM_AssaultRifle_Projectile.SM_AssaultRifle_Projectile'"));

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
	ImpactFinder(TEXT("SoundCue'/Game/Weapons/Sound/Rifle_BulletImpact_Cue.Rifle_BulletImpact_Cue'"));

	if (ImpactFinder.Succeeded())
	{
		Impact = ImpactFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *ImpactFinder.GetReferencerName());
	}
}

void ACI541Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((Expiration -= DeltaTime) <= 0.0f)
	{
		Expire();
	}
}

void ACI541Projectile::OnHit(UPrimitiveComponent* HitComp,
                             AActor* OtherActor,
                             UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse,
                             const FHitResult& Hit)
{
	if (IsValid(OtherActor) && (OtherActor != this) && IsValid(OtherComp))
	{
		ACI541NPC* NPC = Cast<ACI541NPC>(OtherActor);

		if (IsValid(NPC) && (GetVelocity().SizeSquared() >= 1000.0f))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Impact, GetActorLocation());

			NPC->Damage(Damage);
		}
	}

	if (bExpireOnImpact)
	{
		Expire();
	}
}

bool ACI541Projectile::Expire()
{
	OnExpire();
	return Destroy();
}