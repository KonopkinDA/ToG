#pragma once
#include "FToG_Spell.h"
#include "ToG_FireBallActor.h"

class FToG_FireGun : public FToG_Spell
{
public:

	TArray<AToG_FireBallActor*> FireBallActors;
	double FireGunRadius;

	FToG_FireGun();
	FToG_FireGun(const FToG_FireGun& FireGunSpell);
	FToG_FireGun(FToG_FireGun* FireGunSpell);
	FToG_FireGun(FVector4d FireGunDuration, bool IsFireGunHoldable, int FireGunAnimationId, ESpellPhase SpellStartPhase);

	void SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SetFireGunTransform(AActor* Caster);
	
};
