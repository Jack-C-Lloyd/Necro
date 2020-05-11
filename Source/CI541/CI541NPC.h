// Copyright (c) 2020 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CI541NPC.generated.h"

/**
 * NPCs (Non-Playable Characters) are characters with built-in AI (Artificial Intelligence) logic; the parallel of a player.
 * @see ACharacter, ACI541Player
 */
UCLASS()
class CI541_API ACI541NPC : public ACharacter
{
	GENERATED_BODY()

public:
	ACI541NPC(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
  /** The health of this NPC. */
	UPROPERTY( Category=NPC, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true", ClampMin = 0.0f, ClampMax = 1.0f) )
	float Health = 1.0f;

	/** The damage dealt when this NPC attacks. */
	UPROPERTY( Category=NPC, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true", ClampMin = 0.0f, ClampMax = 1.0f) )
	float AttackDamage = 0.3f;

public:
  /** Get this NPC's health. */
	UFUNCTION( Category=NPC, BlueprintPure )
	FORCEINLINE float GetHealth() const { return Health; }

  /** Damage this NPC. */
	UFUNCTION( Category=NPC, BlueprintCallable )
	void Damage(float InDamage);

	/** Called upon death. */
	UFUNCTION( Category=NPC, BlueprintImplementableEvent )
	void OnDeath();
};

#pragma endregion