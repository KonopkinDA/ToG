#pragma once
#include "FToG_Spell.h"
#include "ToG_FireMeteorActor.h"

class FToG_FireMeteor : public FToG_Spell
{
public:

	AToG_FireMeteorActor* FireMeteorActor;
	double FireMeteorRadius;

	FToG_FireMeteor();
	FToG_FireMeteor(const FToG_FireMeteor& FireMeteorSpell);
	FToG_FireMeteor(FToG_FireMeteor* FireMeteorSpell);
	FToG_FireMeteor(FVector4d FireMeteorDuration, bool IsFireMeteorHoldable, int FireMeteorAnimationId, ESpellPhase SpellStartPhase);

	void SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SetFireMeteorTransform(AActor* Caster);

	FVector3d FindTargetPoint(AActor* Caster);
	
};