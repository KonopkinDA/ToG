#include "FToG_SourceLift.h"

#include "ToG_SourceLiftActor.h"
#include "Engine/SkeletalMeshSocket.h"

FToG_SourceLift::FToG_SourceLift()
{
	SourceLiftActor = nullptr;
}
FToG_SourceLift::FToG_SourceLift(const FToG_SourceLift& SourceLiftSpell) : FToG_Spell(SourceLiftSpell)
{
	SourceLiftActor = SourceLiftSpell.SourceLiftActor;
	
}
FToG_SourceLift::FToG_SourceLift(FToG_SourceLift* SourceLiftSpell) : FToG_Spell(SourceLiftSpell)
{
	SourceLiftActor = SourceLiftSpell->SourceLiftActor;
}

FToG_SourceLift::FToG_SourceLift(FVector4d SourceLiftDuration, bool IsSourceLiftHoldable, int SourceLiftAnimationId, ESpellPhase SpellStartPhase) : FToG_Spell("Source Lift", SourceLiftDuration, IsSourceLiftHoldable, SourceLiftAnimationId, SpellStartPhase)
{
	SourceLiftActor = nullptr;
}


void FToG_SourceLift::SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime)
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

void FToG_SourceLift::SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime)
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

void FToG_SourceLift::SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		FindTargetActor(Caster);
		LiftTarget(Caster);
		break;
	case ETimePhase::Update:
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_SourceLift::SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime)
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

void FToG_SourceLift::FindTargetActor(AActor* Caster)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Caster);
	FHitResult OutHit;
	Caster->GetWorld()->LineTraceSingleByChannel(OutHit,
		Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation(),
		Caster->FindComponentByClass<UCameraComponent>()->GetForwardVector() * 100000 + Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation(),
		ECC_Camera,
		QueryParams);

	if(OutHit.GetActor())
	{
		LiftedActor = OutHit.GetActor();
	}
}

void FToG_SourceLift::LiftTarget(AActor* Caster)
{
	//
	FActorSpawnParameters ActorSpawnParameters;
	SourceLiftActor = Caster->GetWorld()->SpawnActor<AToG_SourceLiftActor>(AToG_SourceLiftActor::StaticClass(), LiftedActor->GetActorLocation(), LiftedActor->GetActorRotation(), ActorSpawnParameters);
	SourceLiftActor->StartLifting(LiftedActor);
	//
}


