// Copyright (c) 2020 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CI541Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UChildActorComponent;
class UCI541Inventory;
class UCI541Item;
/**
 * Players are characters that have a spring-arm, camera, inventory and built-in weapon logic.
 * @see ACharacter, UCI541Inventory, UCI541Item
 */
UCLASS()
class CI541_API ACI541Player : public ACharacter
{
	GENERATED_BODY()

public:
	ACI541Player(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	/** The SprngArm being used for camera placement (by Camera) [non-optional subobject]. */
	UPROPERTY( Category=Player, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	USpringArmComponent* SpringArm = nullptr;

	/** The Camera associated with this player, relative (of SpringArm) [non-optional subobject]. */
	UPROPERTY( Category=Player, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UCameraComponent* Camera = nullptr;

	/** The Inventory associated with this player. */
	UPROPERTY( Category=Player, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UCI541Inventory* Inventory = nullptr;

	/** The weapon held by this player (i.e., ACI541Weapon). */
	UPROPERTY( Category=Player, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UChildActorComponent* ChildActor = nullptr;

public:
	/** Return the SpringArm subobject. */
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }

	/** Name of the SpringArmComponent. */
	static FName SpringArmComponentName;

	/** Name of the SpringArmSocket. Use this name if you want to attach the SpringArm. */
	static FName SpringArmSocketName;

	/** Return the Camera subobject. */
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	/** Name of the CameraComponent. */
	static FName CameraComponentName;

	/** Return the Inventory subobject. */
	FORCEINLINE UCI541Inventory* GetInventory() const { return Inventory; }

	/** Name of the CameraComponent. */
	static FName InventoryComponentName;

	/** Return the ChildActor subobject. */
	FORCEINLINE UChildActorComponent* GetChildActor() const { return ChildActor; }

	/** Name of the ChildActorComponent. */
	static FName ChildActorComponentName;

	/** Name of the ChildActorSocket. */
	static FName ChildActorSocketName;

protected:
	//~ Begin AActor Interface.
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin APawn Interface.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//~ End APawn Interface.

	//~ Begin UInputComponent::ActionBindings Interface.
	void Aim();
	void UnAim();
	void Fire();
	void Reload();
	void ToggleView();
	void ToggleCrouch();
	//~ End UInputComponent::ActionBindings Interface.

	//~ Begin UInputComponent::AxisBindings Interface.
	void Turn(float InAxis);
	void LookUp(float InAxis);
	void MoveRight(float InAxis);
	void MoveForward(float InAxis);
	//~ End UInputComponent::AxisBindings Interface.

private:
	/** Whether this player is aiming. */
	uint32 bIsAiming : 1;

	/** Whether this player is crouching. */
	uint32 bIsCrouching : 1;

	/** Whether this player is crouching. */
	uint32 bIsReloading : 1;

	/** Binding for the reload animatin montage. */
	void Reloaded(UAnimMontage *InAnimMontage, bool InBool);

	/** Either first- or third-person view. */
	uint32 bView : 1;

public:
	/** Is this player aiming? */
	UFUNCTION( Category=Player, BlueprintPure )
	FORCEINLINE bool IsAiming() const { return bIsAiming; }

	/** Is this player crouching? */
	UFUNCTION( Category=Player, BlueprintPure )
	FORCEINLINE bool IsCrouching() const { return bIsCrouching; }

	/** Is this player reloading? */
	UFUNCTION( Category=Player, BlueprintPure )
	FORCEINLINE bool IsReloading() const { return bIsReloading; }

private:
	/** The health of this player. */
	float Health;

	/** The rate of infection over time. */
	UPROPERTY( Category=Player, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true", ClampMin = 0.0f, ClampMax = 1.0f) )
	float InfectionRate = 0.005f;

	/** Whether this player is infected. */
	uint32 bIsInfected : 1;

public:
	/** Get this player's health. */
	UFUNCTION( Category=Player, BlueprintPure )
	FORCEINLINE float GetHealth() const { return Health; }

	/** Damage this player. */
	UFUNCTION( Category=Player, BlueprintCallable )
	void Damage(float InDamage);

	/** Heal this player. */
	UFUNCTION( Category=Player, BlueprintCallable )
	void Heal(float InHealth);

	/** Kill this player. */
	UFUNCTION( Category=Player, BlueprintCallable )
	void Die();

	/** Kill this player. */
	UFUNCTION( Category=Player, BlueprintImplementableEvent )
	void OnDie();

	/** Is this player infected? */
	UFUNCTION( Category=Player, BlueprintPure )
	FORCEINLINE bool IsInfected() const { return bIsInfected; }

	/** Infect this player. */
	UFUNCTION( Category=Player, BlueprintCallable )
	void Infect();

	/** Cure this player of infection. */
	UFUNCTION( Category=Player, BlueprintCallable )
	void Cure();

	//~ Begin UCI541Item Interface.
	UFUNCTION( Category=Player, BlueprintCallable )
	void OnUse(UCI541Item* Item);
	//~ End UCI541Item Interface.
};