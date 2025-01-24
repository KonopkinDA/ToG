// Fill out your copyright notice in the Description page of Project Settings.


#include "ToG_FireBallActor.h"

#include "StaticMeshAttributes.h"
#include "TechnicalFunctions.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AToG_FireBallActor::AToG_FireBallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create mesh and particles for fireball
	FireBallParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireBallParticle"));
	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticle"));
	FireBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireBallMesh"));
	FireBallMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireBallMesh2"));
	//Загрузка меша и текстур 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireParticle(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionLastParticle(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FireMesh(TEXT("/Script/Engine.StaticMesh'/Game/Models/Spells/FireBall.FireBall'"));
	//
	FireBallParticle->SetTemplate(FireParticle.Object);
	ExplosionParticle->SetTemplate(ExplosionLastParticle.Object);
	FireBallMesh->SetStaticMesh(FireMesh.Object);
	FireBallMesh2->SetStaticMesh(FireMesh.Object);
	//
	SetRootComponent(FireBallMesh);
	FireBallParticle->SetupAttachment(RootComponent);
	ExplosionParticle->SetupAttachment(RootComponent);
	FireBallMesh2 -> SetupAttachment(RootComponent);
	//
	FireBallMesh2->SetRelativeRotation(FRotator3d(0,90,0));
	FireBallMesh->SetRelativeScale3D(FVector(0.25f,0.25f,0.25f));
	ExplosionParticle->SetAutoActivate(false);
	FireBallLifePhase = EFireBallLifePhase::Expand;
	FlyTime = 10;
	ExplodeTime = 2.5f;
	Speed = 2000;

}

// Called when the game starts or when spawned
void AToG_FireBallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToG_FireBallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessFireBall(DeltaTime);

}

void AToG_FireBallActor::ProcessFireBall(float DeltaTime)
{
	switch (FireBallLifePhase)
	{
	case EFireBallLifePhase::Hold:
		break;
	case EFireBallLifePhase::Fly:
		if(FlyTime > 0)
		{
			FlyTime -= DeltaTime;
			MoveFireBall(DeltaTime);
			CheckOverlap(DeltaTime);
		}
		else
		{
			Disappear(DeltaTime);
		}
		break;
	case EFireBallLifePhase::Explode:
		if(ExplodeTime > 0)
		{
			ExplodeTime -= DeltaTime;
		}
		else
		{
			Disappear(DeltaTime);
		}
		break;
	}
}

void AToG_FireBallActor::CheckOverlap(float DeltaTime)
{
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape CollisionShape = FCollisionShape().MakeSphere(Radius);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->OverlapMultiByChannel
	(
		OutOverlaps,
		GetActorLocation() + GetActorForwardVector() * (Speed * DeltaTime),
		GetActorRotation().Quaternion(),
		ECC_Camera,
		CollisionShape,
		CollisionQueryParams
	);
	
	for(int i = 0; i < OutOverlaps.Num(); i++)
	{
		if(OutOverlaps[i].GetActor()->ActorHasTag("Player") || OutOverlaps[i].GetActor()->ActorHasTag("PlayerPart"))
		{
			OutOverlaps.RemoveAt(i);
			i--;
		}
	}

	if(OutOverlaps.Num() > 0)
	{
		Speed = 0;
		ExplosionParticle->SetRelativeScale3D(FVector3d(10,10,10));
		ExplosionParticle->ActivateSystem();
		FireBallLifePhase = EFireBallLifePhase::Explode;
		//
		FireBallMesh->SetHiddenInGame(true);
		FireBallMesh2->SetHiddenInGame(true);
		//
		if(OutOverlaps[0].GetActor()->GetComponentByClass<UStaticMeshComponent>() && OutOverlaps[0].GetActor()->GetComponentByClass<UStaticMeshComponent>()->Mobility == EComponentMobility::Movable)
		{
			OutOverlaps[0].GetActor()->GetComponentByClass<UStaticMeshComponent>()->AddImpulseAtLocation(TargetVector * 150000, GetActorLocation());
		}
		FireBallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FireBallMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AToG_FireBallActor::MoveFireBall(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + TargetVector * Speed * DeltaTime);
}

void AToG_FireBallActor::Disappear(float DeltaTime)
{
	Destroy();
}



