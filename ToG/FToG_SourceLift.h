#pragma once
#include "FToG_Spell.h"
#include "ToG_SourceLiftActor.h"

class FToG_SourceLift : public FToG_Spell
{
public:

	AToG_SourceLiftActor* SourceLiftActor;
	AActor* LiftedActor;

	FToG_SourceLift();
	FToG_SourceLift(const FToG_SourceLift& SourceLiftSpell);
	FToG_SourceLift(FToG_SourceLift* SourceLiftSpell);
	FToG_SourceLift(FVector4d SourceLiftDuration, bool IsSourceLiftHoldable, int SourceLiftAnimationId, ESpellPhase SpellStartPhase);

	void SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void FindTargetActor(AActor* Caster);

	void LiftTarget(AActor * Caster);
	
};
