#include "FToG_SwordCombo.h"

FToG_SwordCombo::FToG_SwordCombo(TArray<FString> RequiredComboAbilities, FToG_SwordAttack ComboAbility)
{
	RequiredAbilities = RequiredComboAbilities;
	Ability = ComboAbility;
}

