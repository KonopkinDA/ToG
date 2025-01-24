#pragma once
#include "FToG_Spell.h"
#include "ToG_FireBallActor.h"

class FToG_FireBall : public FToG_Spell
{
public:

	AToG_FireBallActor* FireBallActor;
	double FireBallExpandSpeed;
	double FireBallScale;
	double FireBallRadius;

	FToG_FireBall();
	FToG_FireBall(const FToG_FireBall& FireBallSpell);
	FToG_FireBall(FToG_FireBall* FireBallSpell);
	FToG_FireBall(FVector4d FireBallDuration, bool IsFireBallHoldable, int FireBallAnimationId, ESpellPhase SpellStartPhase);

	void SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime) override;

	void SetFireBallTransform(AActor* Caster);
	
};
