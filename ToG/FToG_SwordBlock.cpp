#include "FToG_SwordBlock.h"

FToG_SwordBlock::FToG_SwordBlock()
{
}

FToG_SwordBlock::FToG_SwordBlock(const FToG_SwordBlock& SwordBlock)
{
	Name = SwordBlock.Name;
	Duration = SwordBlock.Duration;
	FullDuration = SwordBlock.FullDuration;
}

FToG_SwordBlock::FToG_SwordBlock(FToG_SwordBlock * SwordBlock)
{
	Name = SwordBlock->Name;
	Duration = SwordBlock->Duration;
	FullDuration = SwordBlock->FullDuration;
}

FToG_SwordBlock::FToG_SwordBlock(FString BlockName, FVector4d BlockDuration)
{
	Name = BlockName;
	Duration = BlockDuration;
	FullDuration = BlockDuration;
}
