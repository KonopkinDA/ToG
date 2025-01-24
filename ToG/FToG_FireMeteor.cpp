#include "FToG_FireMeteor.h"

#include "ToG_FireMeteorActor.h"
#include "Engine/SkeletalMeshSocket.h"

FToG_FireMeteor::FToG_FireMeteor()
{
	FireMeteorActor = nullptr;
}
FToG_FireMeteor::FToG_FireMeteor(const FToG_FireMeteor& FireMeteorSpell) : FToG_Spell(FireMeteorSpell)
{
	FireMeteorActor = nullptr;
}
FToG_FireMeteor::FToG_FireMeteor(FToG_FireMeteor* FireMeteorSpell) : FToG_Spell(FireMeteorSpell)
{
	FireMeteorActor = nullptr;
}

FToG_FireMeteor::FToG_FireMeteor(FVector4d FireMeteorDuration, bool IsFireMeteorHoldable, int FireMeteorAnimationId, ESpellPhase SpellStartPhase) : FToG_Spell("Fire Meteor", FireMeteorDuration, IsFireMeteorHoldable, FireMeteorAnimationId, SpellStartPhase)
{
	FireMeteorActor = nullptr;
	FireMeteorRadius = 50;
}

void FToG_FireMeteor::SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		if(!FireMeteorActor)
		{
			//
			FActorSpawnParameters ActorSpawnParameters;
			FireMeteorActor = Caster->GetWorld()->SpawnActor<AToG_FireMeteorActor>(AToG_FireMeteorActor::StaticClass(), Caster->GetActorLocation(), Caster->GetActorRotation(), ActorSpawnParameters);
			//
			SetFireMeteorTransform(Caster);
			//
			FireMeteorActor->SetActorScale3D(FVector3d(0,0,0));
			FireMeteorActor->FireMeteorLifePhase = EFireMeteorLifePhase::Hold;
			FireMeteorActor->TargetPoint = Caster->GetActorLocation();
		}
		else
		{
			//
			SetFireMeteorTransform(Caster);
			//
			FireMeteorActor->SetActorScale3D(FVector3d(3,3,3));
		}
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_FireMeteor::SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_FireMeteor::SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		FireMeteorActor->TargetPoint = FindTargetPoint(Caster);
		break;
	case ETimePhase::Update:
		SetFireMeteorTransform(Caster);
		break;
	case ETimePhase::Final:
		SetFireMeteorTransform(Caster);
		FireMeteorActor->FireMeteorLifePhase = EFireMeteorLifePhase::Fly;
		FireMeteorActor->Radius = FireMeteorRadius;
		FireMeteorActor->TargetVector = (FireMeteorActor->TargetPoint - FireMeteorActor->GetActorLocation()).GetSafeNormal();
		FireMeteorActor->FlyDistance = (FireMeteorActor->GetActorLocation() - FireMeteorActor->TargetPoint).Length();
		break;
	}
}

void FToG_FireMeteor::SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		break;
	case ETimePhase::Final:
		break;
	}
}


void FToG_FireMeteor::SetFireMeteorTransform(AActor* Caster)
{
	FireMeteorActor->SetActorLocation(FireMeteorActor->TargetPoint + FVector3d().UpVector * 3000 + Caster->GetActorRightVector() * FMath().RandRange(-3000,3000));
	FireMeteorActor->SetActorRotation(Caster->FindComponentByClass<UCameraComponent>()->GetComponentRotation());
}

FVector3d FToG_FireMeteor::FindTargetPoint(AActor* Caster)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Caster);
	FHitResult OutHit;
	Caster->GetWorld()->LineTraceSingleByChannel(OutHit,
		Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation(),
		Caster->FindComponentByClass<UCameraComponent>()->GetForwardVector() * 100000 + Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation(),
		ECC_Camera,
		QueryParams);

	return OutHit.Location;
}