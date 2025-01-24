#include "ToG_CommonAbstractions.h"

LocationRotationOffset::LocationRotationOffset()
{
	Location = FVector3d().Zero();
	Rotation = FVector3d().Zero();
}

LocationRotationOffset::LocationRotationOffset(FVector3d OffsetLocation, FVector3d OffsetRotation)
{
	Location = OffsetLocation;
	Rotation = OffsetRotation;
}
