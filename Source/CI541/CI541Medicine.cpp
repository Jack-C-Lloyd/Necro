// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

#include "CI541Medicine.h"
#include "CI541Player.h"
#include "CI541Inventory.h"

UCI541Medicine::UCI541Medicine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = 0;
	bCanCure = false;
}

void UCI541Medicine::Use(class ACI541Player* Player)
{
	if (IsValid(Player))
	{
		Player->Heal(Health);

		if (bCanCure)
		{
			Player->Cure();
		}

		if (IsValid(Container))
		{
			Container->Remove(this);
		}
	}
}