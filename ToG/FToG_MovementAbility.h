#pragma once
#include "ToG_CommonAbstractions.h"
#include "FTog_Ability.h"

class FToG_MovementAbility : public FTog_Ability
{
public:
	FVector4d Duration;
	FVector4d FullDuration;
	int PawnAnimationId;
	int CameraAnimationId;
	EMovementDirection Direction;
	

	FToG_MovementAbility();
	FToG_MovementAbility(const FToG_MovementAbility& MovementAbility);
	FToG_MovementAbility(FToG_MovementAbility * MovementAbility);
	FToG_MovementAbility(FString MovementName, FVector4d MovementDuration,
		EMovementDirection MovementDirection, int MovementAnimationId);
};
