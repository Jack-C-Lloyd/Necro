// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CI541Item.generated.h"

class ACI541Player;
class UCI541Inventory;
class UStaticMesh;
class UTexture2D;

/**
 * Items are objects that have a title, description, thumbnail, and built-in inventory logic.
 * @see UObject, UCI541Inventory, ACI541Player
 */
UCLASS( Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew )
class CI541_API UCI541Item : public UObject
{
	GENERATED_BODY()
	
public:
  UCI541Item(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
  /** The title of this item (e.g., "Assault Rifle Ammunition") [non-optional]. */
	UPROPERTY( Category=Item, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	FText Title;

  /** The description of this item (e.g., "Ammunition for an assault rifle.") [non-optional]. */
	UPROPERTY( Category=Item, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true", MultiLine = "true") )
	FText Description;

	/** The thumbnail (texture) associated with this item [non-optional subobject]. */
	UPROPERTY( Category=Item, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UTexture2D* Thumbnail;

public:
	/** Return the Title. */
	FORCEINLINE FText GetTitle() const { return Title; }

	/** Return the Description. */
	FORCEINLINE FText GetDescription() const { return Description; }

	/** Return the Thumbnail subobject. */
	FORCEINLINE UTexture2D* GetThumbnail() const { return Thumbnail; }

protected:
	/** The inventory that contains this item. */
	UPROPERTY( BlueprintReadOnly, meta=(AllowPrivateAccess = "true") )
	UCI541Inventory* Container;

public:
	/** Return the inventory containing this item. */
	UFUNCTION( Category=Item, BlueprintPure )
	FORCEINLINE UCI541Inventory* GetContainer() const { return	Container; }

	/**
	 * Try to set the inventory containing this item.
	 * @param InInventory The inventory to contain this item.
	 * @return {@code true} on success, otherwise {@code false}.
	 */
	UFUNCTION( Category=Item, BlueprintCallable )
	bool SetContainer(UCI541Inventory* InInventory);

	/** Set this item to have no container. */
	UFUNCTION( Category=Item, BlueprintCallable )
	void NoContainer();

protected:
	/** Whether this item is stackable. */
	UPROPERTY( Category=Item, EditDefaultsOnly, meta=(AllowPrivateAccess = "true") )
	uint32 bIsStackable : 1;

	/** If the item is stackable, the stack. */
	UPROPERTY( Category=Item, EditDefaultsOnly, meta=(AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 99) )
	int32 Stack;

public:
	/** Is it stackable?. */
	UFUNCTION( Category=Item, BlueprintPure )
	FORCEINLINE bool IsStackable() const { return bIsStackable; }

	/** Return the stack. */
	UFUNCTION( Category=Item, BlueprintPure )
	FORCEINLINE int32 GetStack() const { return Stack; }

	/** Return if the stack is empty. */
	UFUNCTION( Category=Item, BlueprintPure )
	FORCEINLINE int32 IsEmpty() const { return Stack <= 0; }

	/**
	 * Try to increment (++) this item's stack.
	 * @return {@code true} on success, otherwise {@code false}.
	 */
	UFUNCTION( Category=Item, BlueprintCallable )
	bool Increment();

	/**
	 * Try to decrement (--) this item's stack.
	 * @return {@code true} on success, otherwise {@code false}.
	 */
	UFUNCTION( Category=Item, BlueprintCallable )
	bool Decrement();

	/**
	 * The C++ use function.
	 * @param Player The player trying to use this item.
	 */
	virtual void Use(ACI541Player* InPlayer);

	/**
	 * The Blueprints use function.
	 * @param Player The player trying to use this item.
	 */
	UFUNCTION( Category=Item, BlueprintImplementableEvent )
	void OnUse(ACI541Player* InPlayer);
};

#pragma endregion