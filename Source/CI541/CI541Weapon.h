// Copyright (c) 2019-2020 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI541Weapon.generated.h"

class UAnimMontage;
class UCI541Inventory;
class UParticleSystemComponent;
class USkeletalMeshComponent;
class USoundBase;

/**
 * Weapons are actors that have a mesh, particles, sound, and built-in ammunition logic.
 * @see AActor, UCI541Ammunition, ACI541Projectile
 */
UCLASS()
class CI541_API ACI541Weapon : public AActor
{
	GENERATED_BODY()

public:
	ACI541Weapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	/** The skeletal mesh associated with this weapon [non-optional subobject]. */
	UPROPERTY( Category=Weapon, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	/** The particle system associated with this weapon [non-optional subobject]. */
	UPROPERTY( Category=Weapon, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UParticleSystemComponent* ParticleSystem = nullptr;
	
	/** The audio associated with this Weapon [non-optional subobject]. */
	UPROPERTY( Category=Weapon, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	USoundBase* SoundBase;

	/** The reload animation montage associated with this Weapon [non-optional subobject]. */
	UPROPERTY( Category=Weapon, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UAnimMontage* AnimMontage;

public:
	/** Return the SkeletalMesh subobject. */
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return SkeletalMesh; }

	/** Name of the SkeletalMeshComponent. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName SkeletalMeshComponentName;

	/** Return the ParticleSystem subobject. */
	FORCEINLINE UParticleSystemComponent* GetParticleSystem() const { return ParticleSystem; }

	/** Name of the ParticleSystemComponent. */
	static FName ParticleSystemComponentName;

	/** Name of the ParticleSystemSocket. Use this name if you want to attach and/or spawn the ParticleSystem. */
	static FName ParticleSystemSocketName;

	/** Return the SoundBase subobject. */
	FORCEINLINE USoundBase* GetSoundBase() const { return SoundBase; }

	/** Name of the SoundBaseComponent. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName SoundBaseComponentName;

	/** Return the AnimMontage subobject. */
	FORCEINLINE UAnimMontage* GetAnimMontage() const { return AnimMontage; }

private:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface.

	/**
	 * The type of ammunition required to fire or reload.
	 * @see UCI541Ammunition
	 */
	UPROPERTY( Category=Ammunition, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	class TSubclassOf<class UCI541Ammunition> Ammunition;

	/**
	 * The type of projectile to spawn.
	 * @see ACI541Projectile
	 */
	UPROPERTY( Category=Projectile, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	class TSubclassOf<class ACI541Projectile> Projectile;

public:
	/* The maximum load capacity for this weapon. */
	UPROPERTY( Category=Weapon, EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0, ClampMax = 99) )
	int32 Capacity = 10;

	/* The amount of ammunition loaded in this weapopon. */
	UPROPERTY( Category=Weapon, EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0, ClampMax = 99) )
	int32 Loaded;

	/*
	 * Try to fire the weapon; dependent on the loaded ammunition.
	 * @return {@code true} on success, otherwise {@code false}.
	 */
	UFUNCTION( Category=Weapon, BlueprintCallable )
	bool Fire();

	/**
	 * Calleed when this weapon successfully fires.
	 * @see ACI541Weapon::Fire()
	 */
	UFUNCTION( Category=Weapon, BlueprintImplementableEvent )
	void OnFire();

	/*
	 * Try to reload the weapon.
	 * @param InInventory the inventory to try and consume ammunition from.
	 * @return {@code true} on success, otherwise {@code false}.
	 * @see UCI541Inventory
	 */
	UFUNCTION( Category=Weapon, BlueprintCallable )
	bool Reload(UCI541Inventory* InInventory);

	/**
	 * Calleed when this weapon successfully reloads.
	 * @see ACI541Weapon::Reload()
	 */
	UFUNCTION( Category=Weapon, BlueprintImplementableEvent )
	void OnReload();
};