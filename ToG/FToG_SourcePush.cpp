#include "FToG_SourcePush.h"
//#include "G:\Unreal\Projects\ToG_54\Intermediate\Build\Win64\x64\ToGEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "Engine/SkeletalMeshSocket.h"

FToG_SourcePush::FToG_SourcePush()
{
	PushedActor = nullptr;
}
FToG_SourcePush::FToG_SourcePush(const FToG_SourcePush& SourcePushSpell) : FToG_Spell(SourcePushSpell)
{
	PushedActor = nullptr;

}
FToG_SourcePush::FToG_SourcePush(FToG_SourcePush* SourcePushSpell) : FToG_Spell(SourcePushSpell)
{
	PushedActor = nullptr;
}

FToG_SourcePush::FToG_SourcePush(FVector4d SourcePushDuration, bool IsSourcePushHoldable, int SourcePushAnimationId, ESpellPhase SpellStartPhase) : FToG_Spell("Fire Ball", SourcePushDuration, IsSourcePushHoldable, SourcePushAnimationId, SpellStartPhase)
{
	PushedActor = nullptr;
	PushForce = 200000;
}


void FToG_SourcePush::SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime)
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

void FToG_SourcePush::SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime)
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

void FToG_SourcePush::SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		FindTargetActor(Caster);
		PushTarget(Caster);
		break;
	case ETimePhase::Update:
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_SourcePush::SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime)
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

void FToG_SourcePush::FindTargetActor(AActor* Caster)
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
		PushedActor = OutHit.GetActor();
		PushedPoint = OutHit.Location;
	}
}

void FToG_SourcePush::PushTarget(AActor* Caster)
{
	if(PushedActor)
	{
		FVector3d ForceVector = (PushedActor->GetActorLocation() - Caster->GetActorLocation()).GetSafeNormal();
		if(PushedActor->GetComponentByClass<UStaticMeshComponent>() && PushedActor->GetComponentByClass<UStaticMeshComponent>()->Mobility == EComponentMobility::Movable)
		{
			PushedActor->GetComponentByClass<UStaticMeshComponent>()->AddImpulseAtLocation(ForceVector * PushForce, PushedActor->GetActorLocation());	
		}
	}
}


