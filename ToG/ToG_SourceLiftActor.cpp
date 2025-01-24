#include "ToG_SourceLiftActor.h"
//#include "G:\Unreal\Projects\ToG_54\Intermediate\Build\Win64\x64\ToGEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "TechnicalFunctions.h"


// Sets default values
AToG_SourceLiftActor::AToG_SourceLiftActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	LiftPhase = Zero;
	LiftHeight = 150;
	LiftSpeed = 1000;
	LiftAcceleration = 1000;
	LiftShakingDistance = 10;
	LiftShakingTime = 2;
	LiftShakingSpeed = 10;

}

// Called when the game starts or when spawned
void AToG_SourceLiftActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToG_SourceLiftActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessLiftActor(DeltaTime);
}

void AToG_SourceLiftActor::ProcessLiftActor(float DeltaTime)
{
	switch (LiftPhase)
	{
	case Zero:
		break;
	case Lifting:
		LiftTarget(DeltaTime);
		break;
	case Shaking:
		ShakeTarget(DeltaTime);
		break;
	case Droping:
		DropTarget(DeltaTime);
		break;
	}
}


void AToG_SourceLiftActor::LiftTarget(float DeltaTime)
{
	double NextHeight = LiftedActor->GetActorLocation().Z + LiftSpeed * DeltaTime;
	if(NextHeight < LiftedActorStartPosition.Z + LiftHeight)
	{
		LiftedActor->SetActorLocation(LiftedActor->GetActorLocation() + FVector3d(0,0,LiftSpeed * DeltaTime));
		LiftSpeed += LiftAcceleration * DeltaTime;
	}
	else
	{
		LiftedActor->SetActorLocation(FVector3d(LiftedActor->GetActorLocation().X, LiftedActor->GetActorLocation().Y, LiftedActorStartPosition.Z + LiftHeight));
		LiftPhase = Shaking;
	}
}

void AToG_SourceLiftActor::ShakeTarget(float DeltaTime)
{
	if(LiftShakingTime > 0)
	{
		double TargetHeight = LiftedActorStartPosition.Z + LiftHeight;
		LiftedActor->SetActorLocation(LiftedActor->GetActorLocation() + FVector3d(0,0,LiftShakingSpeed * DeltaTime));
		if(LiftedActor->GetActorLocation().Z > TargetHeight + LiftShakingDistance)
		{
			LiftShakingSpeed *= -1;
		}
		if(LiftedActor->GetActorLocation().Z < TargetHeight - LiftShakingDistance)
		{
			LiftShakingSpeed *= -1;
		}
		LiftShakingTime -= DeltaTime;
	}
	else
	{
		LiftPhase = Droping;
	}
}

void AToG_SourceLiftActor::DropTarget(float DeltaTime)
{
	LiftedActor->GetComponentByClass<UStaticMeshComponent>()->SetEnableGravity(true);
	Destroy();
}

void AToG_SourceLiftActor::StartLifting(AActor* SpellLiftedActor)
{
	LiftedActor = SpellLiftedActor;
	LiftedActor->GetComponentByClass<UStaticMeshComponent>()->SetEnableGravity(false);
	LiftedActorStartPosition = SpellLiftedActor->GetActorLocation();
	LiftPhase = Lifting;
}

