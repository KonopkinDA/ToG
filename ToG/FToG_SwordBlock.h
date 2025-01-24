#pragma once
#include "FTog_Ability.h"

class FToG_SwordBlock : public FTog_Ability
{
public:
	
	FVector4d Duration;
	FVector4d FullDuration;

	FToG_SwordBlock();
	FToG_SwordBlock(const FToG_SwordBlock& SwordBlock);
	FToG_SwordBlock(FToG_SwordBlock * SwordBlock);
	FToG_SwordBlock(FString BlockName, FVector4d BlockDuration);
	
};
