#include "FToG_MotionSequence.h"

FToG_MotionSequence::FToG_MotionSequence()
{

}

FToG_MotionSequence::FToG_MotionSequence(
	FString MSName, float MSDuration,
	TArray<FVector3d> MSLocationSequences, TArray<FVector3d> MSRotationSequences)
{
	Name = MSName;
	Duration = MSDuration;
	LocationSequences = MSLocationSequences;
	RotationSequences = MSRotationSequences;
	CurrentDuration = 0;
	CurrentOffsetPosition = FVector3d().Zero();
	CurrentOffsetRotation = FVector3d().Zero();
	BasisVectorX = FVector3d().ForwardVector;
	BasisVectorY = FVector3d().RightVector;
	BasisVectorZ = FVector3d().UpVector;
}

bool FToG_MotionSequence::Play(AActor* MotionPawn, float DeltaTime)
{
	if(CurrentDuration + DeltaTime < Duration)
	{
		CurrentDuration += DeltaTime;
		//Position
		NewMovement = FindOffset(LocationSequences, CurrentOffsetPosition);
		CurrentOffsetPosition += NewMovement;
		NewMovement = BasisVectorX * NewMovement.X + BasisVectorY * NewMovement.Y + BasisVectorZ * NewMovement.Z;
		MotionPawn->SetActorLocation(MotionPawn->GetActorLocation() + NewMovement);
		//Rotation
		NewRotation = FindOffset(RotationSequences, CurrentOffsetRotation);
		CurrentOffsetRotation += NewRotation;
		MotionPawn->SetActorRotation(MotionPawn->GetActorRotation() + FRotator(NewRotation.Y, NewRotation.Z, NewRotation.X));
		
		return false;
	}
	else
	{
		//
		NewMovement = LocationSequences.Last() - CurrentOffsetPosition;
		NewMovement = BasisVectorX * NewMovement.X + BasisVectorY * NewMovement.Y + BasisVectorZ * NewMovement.Z;
		MotionPawn->SetActorLocation(MotionPawn->GetActorLocation() + NewMovement);
		//
		NewRotation = RotationSequences.Last() - CurrentOffsetRotation;
		MotionPawn->SetActorRotation(MotionPawn->GetActorRotation() + FRotator(NewRotation.Y, NewRotation.Z, NewRotation.X));
		
		return true;
	}
}

void FToG_MotionSequence::SetBasis(FVector3d MSForwardVector, FVector3d MSRightVector, FVector3d MSUpVector)
{
	BasisVectorX = MSForwardVector;
	BasisVectorY = MSRightVector;
	BasisVectorZ = MSUpVector;
}

void FToG_MotionSequence::Adjust(float Distance, float TargetDistance, float Angle)
{
	int a;
	if(Angle > 0)
	{
		a = -1;
	}
	else
	{
		a = 1;
	}
	
	int FullFramesPassed = 0;
	
	for(int i = 0; i < LocationSequences.Num(); i++)
	{
		if(LocationSequences[i].X < TargetDistance - Distance)
		{
			FullFramesPassed++;
		}
		else
		{
			FullFramesPassed--;
			break;
		}
	}

	if(FullFramesPassed + 1 < LocationSequences.Num())
	{
		if(FullFramesPassed >= 0)
		{
			float c = (TargetDistance - Distance - LocationSequences[FullFramesPassed].X) / (LocationSequences[FullFramesPassed + 1].X - LocationSequences[FullFramesPassed].X);
			float OneFrameTime = Duration / (LocationSequences.Num() - 1);
			CurrentDuration = (FullFramesPassed + c) * OneFrameTime;
			CurrentOffsetPosition.X = TargetDistance - Distance;
		}
		else
		{
			CurrentDuration = 0;
		}	
	}
	else
	{
		CurrentDuration = Duration;
		CurrentOffsetPosition.X = TargetDistance - Distance;
	}
}

FVector3d FToG_MotionSequence::FindOffset(TArray<FVector3d> TargetVector, FVector3d CurrentOffset)
{
	float OneFrameTime = Duration / (TargetVector.Num() - 1);
	int FullFramesPassed = (int)(CurrentDuration / OneFrameTime);
	float CurrentPointOnFrame = CurrentDuration / OneFrameTime - FullFramesPassed;
	// X
	float x = (TargetVector[FullFramesPassed+1].X - TargetVector[FullFramesPassed].X) * CurrentPointOnFrame + TargetVector[FullFramesPassed].X;
	// Y
	float y = (TargetVector[FullFramesPassed+1].Y - TargetVector[FullFramesPassed].Y) * CurrentPointOnFrame + TargetVector[FullFramesPassed].Y;
	// Z
	float z = (TargetVector[FullFramesPassed+1].Z - TargetVector[FullFramesPassed].Z) * CurrentPointOnFrame + TargetVector[FullFramesPassed].Z;

	return FVector3d(x - CurrentOffset.X,y - CurrentOffset.Y,z - CurrentOffset.Z);
}

