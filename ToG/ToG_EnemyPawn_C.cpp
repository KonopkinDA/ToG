// Fill out your copyright notice in the Description page of Project Settings.


#include "ToG_EnemyPawn_C.h"

#include <string>

#include "ToG_Pawn_C.h"

// Sets default values
AToG_EnemyPawn_C::AToG_EnemyPawn_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create static mesh for pawn
    PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	//Make the pawn's mesh a root component 
	SetRootComponent(PawnMesh);
	//
	PawnBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyBody"));
	//
	PawnBody->SetupAttachment(RootComponent);
	//
	CameraTargetPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraTargetPoint"));
	//
	CameraTargetPoint->SetupAttachment(RootComponent);
	//
	EnemySwordAttackType = ESwordAttackType::Parriable;
	//
	EnemyOnSwordAction = false;
	//
	EnemySwordAttackDirection = ESwordAttackDirection::Nothing;
	//
	IsHitByEnemy = false;
	//
	IsAttackParried = false;
	//
	CameraTargetPoint->SetRelativeLocation(FVector3d(0,0,40));
	//
	AnimationStartPosition = 0.0;
	//
	AroundRotationSpeed = 100.0f;
	//
	SwordAttackPhase = ESwordAttackPhase::Zero;
	//
	CanHit = false;
	//
	AttackTime = 1;
	//
	CurrentAttackTime = 0;
	//
	CombatDistance = 200.0f;
	//
	FollowSpeed = 350.0f;
	//
	FollowAttackSpeed = 200.0f;
	//
	CurrentSpeed = 350.0f;
}

// Called when the game starts or when spawned
void AToG_EnemyPawn_C::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToG_EnemyPawn_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerPawn)
	{
		FollowTarget(DeltaTime);
		AttackTarget(DeltaTime);
	}

}

//
bool AToG_EnemyPawn_C::HitBySword(FToG_SwordAttack SwordAbility, AToG_Pawn_C* HitPawn)
{
	IsHitByEnemy = true;
	HitDirection = SwordAbility.Direction;
	if(SwordAbility.Name == "Light up attack")
	{
		return true;
	}
	else
	{
		return  false;
	}
}

//Rotate pawn around current combat target
void AToG_EnemyPawn_C::RotatePawnAroundTarget(FVector Start, AActor* Player)
{
	//
	FRotator Rotation = FRotator().ZeroRotator;
	//
	float Difference;
	//Calculate rotation between current pawn's rotation and required rotation that looks at target
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Start,Player->GetActorLocation());
	//Determine the default direction of rotation (target on the right side)
	int SideModifier = 1;
	//Change direction of rotation if it necessary (target on the left side)
	if(GetAngleBetweenVectorsWithoutZ(this->GetActorRightVector(), (Player->GetActorLocation() - this->GetActorLocation())) > 90)
	{
		SideModifier = -1;
	}
	//Process cases when difference between rotations if low, but they have different sign
	if(NewRotation.Yaw > 0 && GetActorRotation().Yaw < 0 || NewRotation.Yaw < 0 && GetActorRotation().Yaw > 0)
	{
		if(abs(NewRotation.Yaw) > 90)
		{
			Difference = 180 - abs(NewRotation.Yaw) + 180 - abs(GetActorRotation().Yaw);
		}
		else
		{
			Difference = abs(NewRotation.Yaw) + abs(GetActorRotation().Yaw);
		}
	}
	else
	{
		Difference = abs(NewRotation.Yaw - GetActorRotation().Yaw);
	}
	//If angle between current rotation and required rotation is bigger than speed of rotation and current lock status
	//isn't "true" - rotate with normal speed
	if(Difference > AroundRotationSpeed * GetWorld()->GetDeltaSeconds())
	{
		Rotation.Yaw = SideModifier * AroundRotationSpeed * GetWorld()->GetDeltaSeconds();
	}
	//else - change rotation to required immediately
	else
	{
		Rotation = FRotator(0, NewRotation.Yaw, 0) - GetActorRotation();
	}
	SetActorRotation(GetActorRotation() + Rotation);
}

//
void AToG_EnemyPawn_C::FollowTarget(float DeltaTime)
{
	RotatePawnAroundTarget(GetActorLocation(), PlayerPawn);
	if(GetDistanceTo(PlayerPawn) > CombatDistance)
	{
		FVector3d TargetVector = (PlayerPawn->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		double Movement = CurrentSpeed * DeltaTime;
		if(GetDistanceTo(PlayerPawn) - Movement < CombatDistance)
		{
			Movement = GetDistanceTo(PlayerPawn) - CombatDistance;
		}
		SetActorLocation(GetActorLocation() + TargetVector * Movement);
	}
	else
	{
		if(GetDistanceTo(PlayerPawn) != CombatDistance)
		{
			FVector3d TargetVector = (this->GetActorLocation() - PlayerPawn->GetActorLocation()).GetSafeNormal();
			double Movement = CurrentSpeed * DeltaTime;
			if(GetDistanceTo(PlayerPawn) - Movement > CombatDistance)
			{
				Movement = GetDistanceTo(PlayerPawn) - CombatDistance;
			}
			SetActorLocation(GetActorLocation() + TargetVector * Movement);
		}
	}
}

//
void AToG_EnemyPawn_C::AttackTarget(float DeltaTime)
{
	switch (SwordAttackPhase)
	{
	case ESwordAttackPhase::Zero:
		if(GetDistanceTo(PlayerPawn) <= CombatDistance)
		{
			SwordAttackPhase = ESwordAttackPhase::Attack;
			CurrentAttackTime = DeltaTime;
			CurrentSpeed = FollowAttackSpeed;
		}
		break;
	case ESwordAttackPhase::Attack:
		CurrentAttackTime += DeltaTime;
		if(CurrentAttackTime >= AttackTime)
		{
			SwordAttackPhase = ESwordAttackPhase::Zero;
			CurrentSpeed = FollowSpeed;
		}
		break;
	}
}


