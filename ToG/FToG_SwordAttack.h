#pragma once

#include "ToG_CommonAbstractions.h"
#include "FToG_MotionSequence.h"
#include "FTog_Ability.h"

class FToG_SwordAttack : public FTog_Ability
{
public:
	FVector4d Duration;
	FVector4d FullDuration;
	int AnimationId;
	double Damage;
	ESwordAttackType Type;
	ESwordAttackDirection Direction;
	TArray<UAnimSequence*> AnimationSequences;
	

	FToG_SwordAttack();
	FToG_SwordAttack(const FToG_SwordAttack& SwordAbility);
	FToG_SwordAttack(FToG_SwordAttack * SwordAbility);
	FToG_SwordAttack(FString AttackName, FVector4d AttackDuration ,double AttackDamage, ESwordAttackType AttackType,
		ESwordAttackDirection AttackDirection, int AttackAnimationId, TArray<UAnimSequence*> AttackAnimationSequence);
};
