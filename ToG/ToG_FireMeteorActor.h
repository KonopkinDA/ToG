#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToG_FireMeteorActor.generated.h"

UENUM(BlueprintType)
enum class EFireMeteorLifePhase : uint8
{
	Hold, Fly, Explode
};

UCLASS()
class TOG_API AToG_FireMeteorActor : public AActor
{
	GENERATED_BODY()
	
public:
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* FireMeteorMesh;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystemComponent* FireMeteorParticle;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystemComponent* ExplosionParticle;
	//
	EFireMeteorLifePhase FireMeteorLifePhase;
	//
	FVector3d TargetVector;
	FVector3d TargetPoint;
	double FlyDistance;
	double ExplodeTime;
	double Speed;
	double Acceleration;;
	double Radius;
	double ExplosionRadius;
	double ExplosionDistanceCoefficient;
	double ExplosionFullForce;
	// Sets default values for this actor's properties
	AToG_FireMeteorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ProcessFireMeteor(float DeltaTime);

	void Explode();

	void BlastWave();

	void Disappear(float DeltaTime);

	void MoveFireMeteor(float DeltaTime);

};
