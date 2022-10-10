// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CI541Projectile.h"
#include "CI541Explosive.generated.h"

class UParticleSystemComponent;
class USoundBase;

/**
 * Explosives are a type of projectile (i.e., actors that have a mesh, collision, and built-in projectile movement logic) that have particles and sound.
 * @see ACI541Projectile
 */
UCLASS()
class CI541_API ACI541Explosive : public ACI541Projectile
{
	GENERATED_BODY()
	
public:
  ACI541Explosive(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	/** The particle system associated with this explosive [non-optional subobject]. */
	UPROPERTY( Category=Explosive, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UParticleSystemComponent* ParticleSystem = nullptr;

	/** The audio associated with this explosive [non-optional subobject]. */
	UPROPERTY( Category=Explosive, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	USoundBase* SoundBase;

public:
	/** Return the ParticleSystem subobject. */
	FORCEINLINE UParticleSystemComponent* GetParticleSystem() const { return ParticleSystem; }

	/** Name of the ParticleSystemComponent. */
	static FName ParticleSystemComponentName;

	/** Return the SoundBase subobject */
	FORCEINLINE USoundBase* GetSoundBase() const { return SoundBase; }

	/** Name of the SoundBaseComponent. */
	static FName SoundBaseComponentName;

protected:
	/** The radius of the explosion. */
	UPROPERTY( Category=Explosive, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
		float Radius = 150.0f;

public:
	//~ Begin ACI541Projectile Interface.
	virtual bool Expire() override;
	//~ End ACI541Projectile Interface.
};