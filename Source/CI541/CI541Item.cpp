// Copyright (c) 2020 Jack C. Lloyd. All rights reserved.

#include "CI541Item.h"
#include "CI541Inventory.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UObject/ConstructorHelpers.h"

UCI541Item::UCI541Item(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
  Title = FText::FromString("Assault Rifle Ammunition");
  Description = FText::FromString("Ammunition for an assault rifle.");

  static ConstructorHelpers::FObjectFinder<UTexture2D>
	ThumbnailFinder(TEXT("Texture2D'/Game/Weapons/Textures/T_AssaultRifle_Thumbnail.T_AssaultRifle_Thumbnail'"));

  if (ThumbnailFinder.Succeeded())
	{
		Thumbnail = ThumbnailFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find \"%s\""), *ThumbnailFinder.GetReferencerName());
	}

	 Container = nullptr;
	 bIsStackable = false;
	 Stack = 0;
}

bool UCI541Item::SetContainer(UCI541Inventory* InInventory)
{
	if (!IsValid(InInventory))
	{
		return false;
	}

	Container = InInventory;

	return true;
}

void UCI541Item::NoContainer()
{
	Container = nullptr;
}

bool UCI541Item::Increment()
{
	if (Stack < 99)
	{
		Stack++;

		return true;
	}

	return false;
}

bool UCI541Item::Decrement()
{
	if (Stack > 0)
	{
		Stack--;

		return true;
	}

	return false;
}

void UCI541Item::Use(ACI541Player* Player)
{
  ;
}