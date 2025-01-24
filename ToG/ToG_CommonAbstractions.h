#pragma once

UENUM()
enum class ESwordAttackDirection : uint8
{
	Up,
	Down,
	Right,
	Left,
	Nothing
};

UENUM()
enum class ESwordAttackType : uint8
{
	Parriable,
	UnParriable
};

UENUM()
enum class EPawnControlType : uint8
{
	Free,
	Animated
};

UENUM(BlueprintType)
enum class ESwordAttackPhase : uint8
{
	Zero, Preparation, Attack, Returning, Rebound
};

UENUM(BlueprintType)
enum class ESwordBlockPhase : uint8
{
	Zero, Block, Hold, Unblock, Hit, Recovery
};

UENUM(BlueprintType)
enum class EDashPhase : uint8
{
	Zero, Dash, Crash
};

UENUM(BlueprintType)
enum class EMovementSide :uint8
{
	Zero,
	Forward,
	Back,
	Right,
	Left,
	ForwardLeft,
	ForwardRight,
	BackLeft,
	BackRight
};

UENUM(BlueprintType)
enum class ESpellPhase :uint8
{
	Zero,
	Cast,
	Hold,
	Using,
	Recovery
};

UENUM(BlueprintType)
enum class ETimePhase : uint8
{
	Start, Update, Final
};


struct LocationRotationOffset
{
	FVector3d Location;
	FVector3d Rotation;

	LocationRotationOffset();
	LocationRotationOffset(FVector3d OffsetLocation, FVector3d OffsetRotation);
};

UENUM(BlueprintType)
enum class EMovementType :uint8
{
	Walk,
	Run,
	Sprint
};

UENUM(BlueprintType)
enum class EMovementDirection :uint8
{
	Zero,
	Forward,
	Back,
	Right,
	Left,
	ForwardRight,
	ForwardLeft,
	BackRight,
	BackLeft
};

UENUM(BlueprintType)
enum class ESpellType :uint8
{
	Source,
	Fire,
	Water,
	Ground,
	Electricity,
	Light,
	Darkness,
	Life,
	Air
};

class ToG_СommonAbstractions
{
public:
	
};
