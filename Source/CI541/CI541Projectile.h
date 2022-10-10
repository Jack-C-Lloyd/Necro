// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI541Projectile.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USoundBase;
class UStaticMeshComponent;

/**
 * Projectiles are actors that have a mesh, collision, and built-in projectile movement logic.
 * @see AActor, UProjectileMovementComponent
 */
UCLASS()
class CI541_API ACI541Projectile : public AActor
{
	GENERATED_BODY()
	
public:
	ACI541Projectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	/** The static mesh associated with this projectile [optional subobject]. */
	UPROPERTY( Category=Projectile, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UStaticMeshComponent* StaticMesh = nullptr;

	/** The BoxComponent used for collision (by ProjectileMovement) [non-optional subobject]. */
	UPROPERTY( Category=Projectile, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UBoxComponent* Box = nullptr;

	/** The ProjectileMovementComponent used for movement (of Box) [non-optional subobject]. */
	UPROPERTY( Category=Projectile, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	/** The audio of impact associated with this projectile (of Box) [non-optional subobject]. */
	UPROPERTY( Category=Projectile, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	USoundBase* Impact = nullptr;

public:
	/** Return the StaticMesh subobject. */
	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return StaticMesh; }

	/** Name of the StaticMeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName StaticMeshComponentName;

	/** Return the Box subobject. */
	FORCEINLINE UBoxComponent* GetBox() const { return Box; }

	/** Name of the BoxComponent. */
	static FName BoxComponentName;

	/** Return the ProjectileMovement subobject. */
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	/** Name of the ProjectileMovementComponent. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName ProjectileMovementComponentName;
	
	//~ Begin AActor Interface.
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface

	//~ Begin UBoxComponent Interface.
	UFUNCTION( Category=Projectile )
	void OnHit(UPrimitiveComponent* HitComp,
	           AActor* OtherActor,
	           UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse,
	           const FHitResult& Hit);
	//~ End UBoxComponent Interface.

protected:
	/** The amount of damage dealt (i.e., within the range of 0.0f and 1.0f). */
	UPROPERTY( Category=Projectile, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true", ClampMin = 0.0f, ClampMax = 1.0f) )
	float Damage = 0.1f;

	/** Seconds until expiration; lifetime. */
	UPROPERTY( Category=Projectile, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	float Expiration = 3.0f;

	/** Whether the projectile expires on collision. */
	UPROPERTY( Category=Projectile, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	uint32 bExpireOnImpact : 1;

public:
	/**
	 * Expire the projectile (i.e., destroy).
	 * @return {@code true} on success, otherwise {@code false}.
	 * @see AActor::Destoy()
	 */
	UFUNCTION( Category=Projectile, BlueprintCallable )
	virtual bool Expire();

	/**
	 * Called when this projectile expires.
	 * @see ACI541Projectile Expire()
	 */
	UFUNCTION( Category=Projectile, BlueprintImplementableEvent )
	void OnExpire();
};