#pragma once

#include "FToG_SwordAttack.h"

class FToG_SwordCombo
{
public:
	TArray<FString> RequiredAbilities;
	FToG_SwordAttack Ability;

	FToG_SwordCombo(TArray<FString> RequiredComboAbilities, FToG_SwordAttack ComboAbility);
	
};
