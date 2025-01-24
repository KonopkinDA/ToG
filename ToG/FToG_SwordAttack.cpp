#include "FToG_SwordAttack.h"

FToG_SwordAttack::FToG_SwordAttack()
{
}

FToG_SwordAttack::FToG_SwordAttack(const FToG_SwordAttack& SwordAbility)
{
	Name = SwordAbility.Name;
	Duration = SwordAbility.Duration;
	FullDuration = SwordAbility.FullDuration;
	Damage = SwordAbility.Damage;
	Type = SwordAbility.Type;
	Direction = SwordAbility.Direction;
	AnimationId = SwordAbility.AnimationId;
	AnimationSequences = SwordAbility.AnimationSequences;
}

FToG_SwordAttack::FToG_SwordAttack(FToG_SwordAttack * SwordAbility)
{
	Name = SwordAbility->Name;
	Duration = SwordAbility->Duration;
	Damage = SwordAbility->Damage;
	Type = SwordAbility->Type;
	Direction = SwordAbility->Direction;
	FullDuration = SwordAbility->FullDuration;
	AnimationId = SwordAbility->AnimationId;
	AnimationSequences = SwordAbility->AnimationSequences;
}

FToG_SwordAttack::FToG_SwordAttack(FString AttackName, FVector4d AttackDuration, double AttackDamage,
	ESwordAttackType AttackType, ESwordAttackDirection AttackDirection, int AttackAnimationId, TArray<UAnimSequence*> AttackAnimationSequences)
{
	Name = AttackName;
	Duration = AttackDuration;
	FullDuration = AttackDuration;
	Damage = AttackDamage;
	Type = AttackType;
	Direction = AttackDirection;
	AnimationId = AttackAnimationId;
	AnimationSequences = AttackAnimationSequences;
}
