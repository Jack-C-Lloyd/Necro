// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CI541Interactable.generated.h"

class UCI541Item;
class USoundBase;
class USphereComponent;
class UStaticMeshComponent;

/**
 * Interactables are Actors that have a mesh, collision, sound, and built-in inventory logic.
 * @see AActor, UCI541Inventory, UCI541Item
 */
UCLASS()
class CI541_API ACI541Interactable : public AActor
{
	GENERATED_BODY()
	
public:
	ACI541Interactable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	/** The static mesh associated with this projectile [optional subobject]. */
	UPROPERTY( Category=Interactable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UStaticMeshComponent* StaticMesh = nullptr;

	/** The SphereComponent used for collision [non-optional subobject]. */
	UPROPERTY( Category=Interactable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	USphereComponent* Sphere = nullptr;

	/** The audio associated with this projectile [non-optional subobject] */
	UPROPERTY( Category=Interactable, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	USoundBase* SoundBase;

	/** The item associated with this interactable [non-optional subobject] */
	UPROPERTY( Category=Interactable, EditDefaultsOnly, Instanced, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UCI541Item* Item = nullptr;

public:
	/** Return the StaticMesh subobject. */
	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return StaticMesh; }

	/** Name of the StaticMeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName StaticMeshComponentName;
	
	/** Return the Sphere subobject. */
	FORCEINLINE USphereComponent* GetSphere() const { return Sphere; }

	/** Name of the SphereComponent. */
	static FName SphereComponentName;

	/** Return the SoundBase subobject. */
	FORCEINLINE USoundBase* GetSoundBase() const { return SoundBase; }

	/** Name of the SoiundBaseComponent. */
	static FName SoundBaseComponentName;

	//~ Begin USphereComponent Interface.
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	                    AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex,
	                    bool bFromSweep,
	                    const FHitResult& SweepResult);
	//~ End USphereComponent Interface.
};