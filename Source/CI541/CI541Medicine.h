// Copyright (c) 2020 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CI541Item.h"
#include "CI541Medicine.generated.h"

class ACI541Player;

/**
 * Medicine is a type of item capable of healing and curing players.
 * @see UCI541Item
 */
UCLASS()
class CI541_API UCI541Medicine : public UCI541Item
{
	GENERATED_BODY()

public:
	UCI541Medicine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	/** The amount of health this medicine heals (i.e., between 0 and 1, inclusive). */
	UPROPERTY( Category=Medicine, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true", ClampMin = 0.0f, ClampMax = 1.0f) )
	float Health;

	/** Whether the medicine cures infection. */
	UPROPERTY( Category=Medicine, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	uint32 bCanCure : 1;

public:
	//~ Begin UCI541Item Interface.
	virtual void Use(ACI541Player* Player) override;
	//~ End UCI541Item Interface.
};