#pragma once
#include "FTog_Ability.h"

class FToG_Spell : public FTog_Ability
{
public:
	FVector4d Duration;
	FVector4d FullDuration;
	bool Holdable;
	int AnimationId;
	ESpellPhase StartPhase;

	FToG_Spell();
	FToG_Spell(const FToG_Spell& Spell);
	FToG_Spell(FToG_Spell* Spell);
	FToG_Spell(FString SpellName, FVector4d SpellDuration, bool IsSpellHoldable, int SpellAnimationId, ESpellPhase SpellStartPhase);
	
	virtual void SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime);

	virtual void SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime);

	virtual void SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime);

	virtual void SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime);
	
};
