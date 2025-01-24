#include "FToG_FireGun.h"

#include "ToG_FireBallActor.h"
#include "Engine/SkeletalMeshSocket.h"

FToG_FireGun::FToG_FireGun()
{
	FireBallActors = TArray<AToG_FireBallActor*>();
	FireGunRadius = 0;
}
FToG_FireGun::FToG_FireGun(const FToG_FireGun& FireGunSpell) : FToG_Spell(FireGunSpell)
{
	FireBallActors = FireGunSpell.FireBallActors;
	FireGunRadius = FireGunSpell.FireGunRadius;
	
}
FToG_FireGun::FToG_FireGun(FToG_FireGun* FireGunSpell) : FToG_Spell(FireGunSpell)
{
	FireBallActors = FireGunSpell->FireBallActors;
	FireGunRadius = FireGunSpell->FireGunRadius;
}

FToG_FireGun::FToG_FireGun(FVector4d FireGunDuration, bool IsFireGunHoldable, int FireGunAnimationId, ESpellPhase SpellStartPhase) : FToG_Spell("Fire Ball", FireGunDuration, IsFireGunHoldable, FireGunAnimationId, SpellStartPhase)
{
	FireBallActors = TArray<AToG_FireBallActor*>();
	FireGunRadius = 50;
}


void FToG_FireGun::SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		// if(!FireGunActor)
		// {
		// 	//
		// 	FActorSpawnParameters ActorSpawnParameters;
		// 	FireGunActor = Caster->GetWorld()->SpawnActor<AToG_FireGunActor>(AToG_FireGunActor::StaticClass(), Caster->GetActorLocation(), Caster->GetActorRotation(), ActorSpawnParameters);
		// 	//
		// 	SetFireGunTransform(Caster);
		// 	//
		// 	FireGunActor->SetActorScale3D(FVector3d(0,0,0));
		// 	FireGunActor->FireGunLifePhase = EFireGunLifePhase::Expand;
		// }
		// else
		// {
		// 	//
		// 	SetFireGunTransform(Caster);
		// 	//
		// 	FireGunActor->SetActorScale3D(FireGunActor->GetActorScale3D() + FVector3d(FireGunExpandSpeed,FireGunExpandSpeed,FireGunExpandSpeed) * DeltaTime);
		// 	FireGunScale += FireGunExpandSpeed * DeltaTime;
		// }
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_FireGun::SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		SetFireGunTransform(Caster);
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_FireGun::SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		SetFireGunTransform(Caster);
		break;
	case ETimePhase::Final:
		// SetFireGunTransform(Caster);
		// FireGunActor->FireGunLifePhase = EFireGunLifePhase::Fly;
		// FireGunActor->Radius = FireGunScale * FireGunRadius;
		// FireGunActor->TargetVector = Caster->FindComponentByClass<UCameraComponent>()->GetForwardVector();
		// FireGunActor->TargetOffsetPosition = Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
		break;
	}
}

void FToG_FireGun::SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime)
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


void FToG_FireGun::SetFireGunTransform(AActor* Caster)
{
	// USkeletalMeshComponent* testSMC = Cast<USkeletalMeshComponent>(Caster->FindComponentByTag(USkeletalMeshComponent::StaticClass(), "Arms"));
	// const USkeletalMeshSocket* testSocket = testSMC->GetSocketByName("fireballsocket");
	// FVector3d testL = testSocket->GetSocketLocation(testSMC);
	// FireGunActor->SetActorLocation(testL);
	// //FireGunActor->SetActorLocation(Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation() + (Caster->FindComponentByClass<UCameraComponent>()->GetForwardVector() * 50 + Caster->FindComponentByClass<UCameraComponent>()->GetUpVector()));
	// FireGunActor->SetActorRotation(Caster->FindComponentByClass<UCameraComponent>()->GetComponentRotation());
}

