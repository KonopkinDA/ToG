#pragma once
#include "FToG_Spell.h"

class FToG_SourcePush : public FToG_Spell
{
public:

	AActor* PushedActor;
	FVector3d PushedPoint;
	double PushForce;
	

	FToG_SourcePush();
	FToG_SourcePush(const FToG_SourcePush& SourcePushSpell);
	FToG_SourcePush(FToG_SourcePush* SourcePushSpell);
	FToG_SourcePush(FVector4d SourcePushDuration, bool IsSourcePushHoldable, int SourcePushAnimationId, ESpellPhase SpellStartPhase);

	void SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void FindTargetActor(AActor* Caster);

	void PushTarget(AActor* Caster);
};
