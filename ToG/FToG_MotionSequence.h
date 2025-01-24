#pragma once

#include "Camera/CameraComponent.h"
#include "ToG_CommonAbstractions.h"
#include "TechnicalFunctions.h"

class FToG_MotionSequence
{
public:
	FString Name;
	float Duration;
	TArray<FVector3d> LocationSequences;
	TArray<FVector3d> RotationSequences;
	FVector3d BasisVectorX;
	FVector3d BasisVectorY;
	FVector3d BasisVectorZ;

	FToG_MotionSequence();
	
	FToG_MotionSequence(
		FString MSName, float MSDuration,
		TArray<FVector3d> MSLocationSequences, TArray<FVector3d> MSRotationSequences
	);

	bool Play(AActor* MotionPawn, float DeltaTime);

	void SetBasis(
		FVector3d MSForwardVector = FVector3d().ForwardVector,
		FVector3d MSRightVector = FVector3d().RightVector,
		FVector3d MSUpVector = FVector3d().UpVector
		);

	void Adjust(float Distance, float TargetDistance, float Angle);

private:
	float CurrentDuration;
	FVector3d CurrentOffsetPosition;
	FVector3d CurrentOffsetRotation;

	FVector3d NewMovement;
	FVector3d NewRotation;

	FVector3d FindOffset(TArray<FVector3d> TargetVector, FVector3d CurrentOffset);
};
