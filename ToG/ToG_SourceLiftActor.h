#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToG_SourceLiftActor.generated.h"

UENUM()
enum ELiftPhase
{
	Zero, Lifting, Shaking, Droping
};

UCLASS()
class TOG_API AToG_SourceLiftActor : public AActor
{

	GENERATED_BODY()
	
public:

	AActor *LiftedActor;
	FVector3d LiftedActorStartPosition;
	ELiftPhase LiftPhase;
	double LiftHeight;
	double LiftSpeed;
	double LiftAcceleration;
	double LiftShakingDistance;
	double LiftShakingTime;
	double LiftShakingSpeed;
	
	AToG_SourceLiftActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ProcessLiftActor(float DeltaTime);
	
	void LiftTarget(float DeltaTime);

	void ShakeTarget(float DeltaTime);

	void DropTarget(float DeltaTime);

	void StartLifting(AActor* SpellLiftedActor);
	
};
