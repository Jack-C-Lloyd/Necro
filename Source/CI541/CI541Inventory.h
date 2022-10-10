// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CI541Inventory.generated.h"

class UCI541Item;

/** Blueprints delegate binding. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

/**
 * Inventories are ActorComponents that hold a limited capacity of items, with built-in management logic.
 * @see UActorComponent, UCI541Item.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CI541_API UCI541Inventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UCI541Inventory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Blueprint delegate binding. */
	UPROPERTY( Category=Inventory, BlueprintAssignable )
	FOnInventoryUpdated OnInventoryUpdated;

private:
	/** The default items contained within this inventory. */
	UPROPERTY( Category=Inventory, EditDefaultsOnly, Instanced, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	TArray<UCI541Item*> Defaults;

	/** The items contained within this inventory. */
	UPROPERTY( Category=Inventory, Instanced, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	TArray<UCI541Item*> Items;

	/** The limited capacity of items */
	UPROPERTY( Category=Inventory, EditDefaultsOnly, meta=(ClampMin = 0) )
	int32 Capacity = 8;

public:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface.

	/**
	 * Try to add an item to this inventory.
	 * @param InItem The item to add.
	 * @return {@code true} on success, otherwise {@code false}.
	 */
	UFUNCTION( Category=Inventory, BlueprintCallable )
	bool Add(UCI541Item* InItem);

	/**
	 * Try to remove an item from this inventory.
	 * @param InItem The item to remove.
	 * @return {@code true} on success, otherwise {@code false}.
	 */
	UFUNCTION( Category=Inventory, BlueprintCallable )
	bool Remove(UCI541Item* InItem);

	/**
	 * Check if this inventory contains a class of item.
	 * @param InClass The item class to compare.
	 * @return The item on success, otherwise {@code nullptr}.
	 */
	UFUNCTION( Category=Inventory, BlueprintCallable )
	UCI541Item* Contains(UClass* InClass);
};