#include "FToG_Spell.h"
#include "TechnicalFunctions.h"

FToG_Spell::FToG_Spell()
{
	
}
FToG_Spell::FToG_Spell(const FToG_Spell& Spell)
{
	Name = Spell.Name;
	Duration = Spell.Duration;
	FullDuration = Spell.FullDuration;
	Holdable = Spell.Holdable;
	AnimationId = Spell.AnimationId;
	StartPhase = Spell.StartPhase;
}
FToG_Spell::FToG_Spell(FToG_Spell* Spell)
{
	Name = Spell->Name;
	Duration = Spell->Duration;
	FullDuration = Spell->FullDuration;
	Holdable = Spell->Holdable;
	AnimationId = Spell->AnimationId;
	StartPhase = Spell->StartPhase;
}
FToG_Spell::FToG_Spell(FString SpellName, FVector4d SpellDuration, bool IsSpellHoldable, int SpellAnimationId, ESpellPhase SpellStartPhase)
{
	Name = SpellName;
	Duration = SpellDuration;
	FullDuration = SpellDuration;
	Holdable = IsSpellHoldable;
	AnimationId = SpellAnimationId;
	StartPhase = SpellStartPhase;
}

void FToG_Spell::SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime)
{

}

void FToG_Spell::SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	
}

void FToG_Spell::SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	
}

void FToG_Spell::SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	
}


