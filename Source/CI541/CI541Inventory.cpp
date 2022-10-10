// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#include "CI541Inventory.h"
#include "CI541Item.h"

UCI541Inventory::UCI541Inventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCI541Inventory::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Default : Defaults)
	{
		Add(Default);
	}
}

bool UCI541Inventory::Add(UCI541Item* InItem)
{
	// If the item is not valid or if the inventory cannot accept it, return false.
	if (!IsValid(InItem) || (!InItem->IsStackable() && Items.Num() >= Capacity))
	{
		return false;
	}

	// Blueprint delegate binding.
	OnInventoryUpdated.Broadcast();

	if (InItem->IsStackable())
	{
		UCI541Item* ItemPtr = Contains(InItem->GetClass());

		if (IsValid(ItemPtr))
		{
			while (InItem->GetStack() > 0)
			{
				if (!ItemPtr->Increment() || !InItem->Decrement())
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			if (Items.Num() >= Capacity)
			{
				return false;
			}
		}
	}
	
	InItem->SetContainer(this);
	Items.Add(InItem);

	return true;
}

bool UCI541Inventory::Remove(UCI541Item* InItem)
{
	if (!IsValid(InItem))
	{
		return false;
	}

	InItem->NoContainer();
	Items.RemoveSingle(InItem);

	/* Blueprint delegate binding. */
	OnInventoryUpdated.Broadcast();

	return true;
}

UCI541Item* UCI541Inventory::Contains(UClass* InClass)
{
	if (IsValid(InClass))
	{
		/* Linear search; expensive, consider refactoring. */
		for (auto& Item : Items)
		{
			if (Item->GetClass() == InClass)
			{
				return Item;
			}
		}
	}

	return nullptr;
}