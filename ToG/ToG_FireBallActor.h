// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToG_FireBallActor.generated.h"

UENUM(BlueprintType)
enum class EFireBallLifePhase : uint8
{
	Expand, Hold, Fly, Explode
};


UCLASS()
class TOG_API AToG_FireBallActor : public AActor
{
	GENERATED_BODY()
	
public:
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* FireBallMesh;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* FireBallMesh2;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystemComponent* FireBallParticle;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystemComponent* ExplosionParticle;
	//
	EFireBallLifePhase FireBallLifePhase;
	//
	FVector3d TargetVector;
	FVector3d TargetOffsetPosition;
	double FlyTime;
	double ExplodeTime;
	double Speed;
	double Radius;
	double HandOffset;
	// Sets default values for this actor's properties
	AToG_FireBallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ProcessFireBall(float DeltaTime);

	void CheckOverlap(float DeltaTime);

	void Disappear(float DeltaTime);

	void MoveFireBall(float DeltaTime);

};
