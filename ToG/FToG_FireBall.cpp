#include "FToG_FireBall.h"

#include "ToG_FireBallActor.h"
#include "Engine/SkeletalMeshSocket.h"

FToG_FireBall::FToG_FireBall()
{
	FireBallActor = nullptr;
	FireBallRadius = 0;
}
FToG_FireBall::FToG_FireBall(const FToG_FireBall& FireBallSpell) : FToG_Spell(FireBallSpell)
{
	FireBallActor = FireBallSpell.FireBallActor;
	FireBallExpandSpeed = FireBallSpell.FireBallExpandSpeed;
	FireBallScale = FireBallSpell.FireBallScale;
	FireBallRadius = FireBallSpell.FireBallRadius;
	
}
FToG_FireBall::FToG_FireBall(FToG_FireBall* FireBallSpell) : FToG_Spell(FireBallSpell)
{
	FireBallActor = FireBallSpell->FireBallActor;
	FireBallExpandSpeed = FireBallSpell->FireBallExpandSpeed;
	FireBallScale = FireBallSpell->FireBallScale;
	FireBallRadius = FireBallSpell->FireBallRadius;
}

FToG_FireBall::FToG_FireBall(FVector4d FireBallDuration, bool IsFireBallHoldable, int FireBallAnimationId, ESpellPhase SpellStartPhase) : FToG_Spell("Fire Ball", FireBallDuration, IsFireBallHoldable, FireBallAnimationId, SpellStartPhase)
{
	FireBallActor = nullptr;
	FireBallExpandSpeed = 0.6;
	FireBallScale = 0;
	FireBallRadius = 50;
}


void FToG_FireBall::SpellCast(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		if(!FireBallActor)
		{
			//
			FActorSpawnParameters ActorSpawnParameters;
			FireBallActor = Caster->GetWorld()->SpawnActor<AToG_FireBallActor>(AToG_FireBallActor::StaticClass(), Caster->GetActorLocation(), Caster->GetActorRotation(), ActorSpawnParameters);
			//
			SetFireBallTransform(Caster);
			//
			FireBallActor->SetActorScale3D(FVector3d(0,0,0));
			FireBallActor->FireBallLifePhase = EFireBallLifePhase::Expand;
		}
		else
		{
			//
			SetFireBallTransform(Caster);
			//
			FireBallActor->SetActorScale3D(FireBallActor->GetActorScale3D() + FVector3d(FireBallExpandSpeed,FireBallExpandSpeed,FireBallExpandSpeed) * DeltaTime);
			FireBallScale += FireBallExpandSpeed * DeltaTime;
		}
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_FireBall::SpellHold(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		SetFireBallTransform(Caster);
		break;
	case ETimePhase::Final:
		break;
	}
}

void FToG_FireBall::SpellUsing(ETimePhase Phase, AActor* Caster, float DeltaTime)
{
	switch (Phase)
	{
	case ETimePhase::Start:
		break;
	case ETimePhase::Update:
		SetFireBallTransform(Caster);
		break;
	case ETimePhase::Final:
		SetFireBallTransform(Caster);
		FireBallActor->FireBallLifePhase = EFireBallLifePhase::Fly;
		FireBallActor->Radius = FireBallScale * FireBallRadius;
		FireBallActor->TargetVector = Caster->FindComponentByClass<UCameraComponent>()->GetForwardVector();
		FireBallActor->TargetOffsetPosition = Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
		break;
	}
}

void FToG_FireBall::SpellRecovery(ETimePhase Phase, AActor* Caster, float DeltaTime)
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


void FToG_FireBall::SetFireBallTransform(AActor* Caster)
{
	//Уязвимость
	USkeletalMeshComponent* testSMC = Cast<USkeletalMeshComponent>(Caster->FindComponentByTag(USkeletalMeshComponent::StaticClass(), "Arms"));
	const USkeletalMeshSocket* testSocket = testSMC->GetSocketByName("fireballsocket");
	FVector3d testL = testSocket->GetSocketLocation(testSMC);
	FireBallActor->SetActorLocation(testL);
	//FireBallActor->SetActorLocation(Caster->FindComponentByClass<UCameraComponent>()->GetComponentLocation() + (Caster->FindComponentByClass<UCameraComponent>()->GetForwardVector() * 50 + Caster->FindComponentByClass<UCameraComponent>()->GetUpVector()));
	FireBallActor->SetActorRotation(Caster->FindComponentByClass<UCameraComponent>()->GetComponentRotation());
}

