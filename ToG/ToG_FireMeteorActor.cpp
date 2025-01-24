#include "ToG_FireMeteorActor.h"

#include "StaticMeshAttributes.h"
#include "TechnicalFunctions.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AToG_FireMeteorActor::AToG_FireMeteorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create mesh and particles for FireMeteor
	FireMeteorParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireMeteorParticle"));
	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticle"));
	FireMeteorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMeteorMesh"));
	//Загрузка меша и текстур 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireParticle(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionLastParticle(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FireMesh(TEXT("/Script/Engine.StaticMesh'/Game/Models/Spells/FireMeteor.FireMeteor'"));
	//
	FireMeteorParticle->SetTemplate(FireParticle.Object);
	ExplosionParticle->SetTemplate(ExplosionLastParticle.Object);
	FireMeteorMesh->SetStaticMesh(FireMesh.Object);
	//
	SetRootComponent(FireMeteorMesh);
	FireMeteorParticle->SetupAttachment(RootComponent);
	ExplosionParticle->SetupAttachment(RootComponent);
	//
	FireMeteorLifePhase = EFireMeteorLifePhase::Hold;
	FireMeteorMesh->SetRelativeScale3D(FVector(1,1,1));
	ExplosionParticle->SetAutoActivate(false);
	ExplodeTime = 2.5f;
	Speed = 0;
	Acceleration = 10000;
	ExplosionRadius = 600;
	ExplosionDistanceCoefficient = 1;
	ExplosionFullForce = 500000;

}

// Called when the game starts or when spawned
void AToG_FireMeteorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToG_FireMeteorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessFireMeteor(DeltaTime);

}

void AToG_FireMeteorActor::ProcessFireMeteor(float DeltaTime)
{
	switch (FireMeteorLifePhase)
	{
	case EFireMeteorLifePhase::Hold:
		break;
	case EFireMeteorLifePhase::Fly:
		if(FlyDistance > 0)
		{
			MoveFireMeteor(DeltaTime);
			Speed += Acceleration * DeltaTime;
		}
		else
		{
			FireMeteorLifePhase = EFireMeteorLifePhase::Explode;
			Explode();
			BlastWave();
		}
		break;
	case EFireMeteorLifePhase::Explode:
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

void AToG_FireMeteorActor::Explode()
{
	Speed = 0;
	ExplosionParticle->SetRelativeScale3D(FVector3d(10,10,10));
	ExplosionParticle->ActivateSystem();
	FireMeteorLifePhase = EFireMeteorLifePhase::Explode;
	//
	FireMeteorMesh->SetHiddenInGame(true);
	//
	FireMeteorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AToG_FireMeteorActor::BlastWave()
{
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape CollisionShape = FCollisionShape().MakeSphere(ExplosionRadius);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->OverlapMultiByChannel
	(
		OutOverlaps,
		GetActorLocation(),
		GetActorRotation().Quaternion(),
		ECC_Camera,
		CollisionShape,
		CollisionQueryParams
	);

	for(int i = 0; i < OutOverlaps.Num(); i++)
	{
		TechnicalFunctionsNamespace::ConsoleLog(OutOverlaps[i].GetActor()->GetName());
		if(OutOverlaps[i].GetActor()->ActorHasTag("Player") || OutOverlaps[i].GetActor()->ActorHasTag("PlayerPart"))
		{
			OutOverlaps.RemoveAt(i);
			i--;
		}
	}

	if(OutOverlaps.Num() > 0)
	{
		//
		for(int i = 0; i< OutOverlaps.Num(); i++)
		{
			FVector3d ForceVector = (OutOverlaps[i].GetActor()->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			if(OutOverlaps[i].GetActor()->GetComponentByClass<UStaticMeshComponent>() && OutOverlaps[i].GetActor()->GetComponentByClass<UStaticMeshComponent>()->Mobility == EComponentMobility::Movable)
			{
				double ExplosionForce = ExplosionFullForce * ExplosionDistanceCoefficient * (ExplosionRadius - (OutOverlaps[i].GetActor()->GetActorLocation() - GetActorLocation()).Length())/ExplosionRadius;
				OutOverlaps[i].GetActor()->GetComponentByClass<UStaticMeshComponent>()->AddImpulseAtLocation(ForceVector * ExplosionForce, GetActorLocation());	
			}
		}
	}
}

void AToG_FireMeteorActor::MoveFireMeteor(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + TargetVector * Speed * DeltaTime);
	FlyDistance -= (TargetVector * Speed * DeltaTime).Length();
}

void AToG_FireMeteorActor::Disappear(float DeltaTime)
{
	Destroy();
}
