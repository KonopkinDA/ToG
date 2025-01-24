// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FToG_SwordAttack.h"
#include "GameFramework/Pawn.h"
#include "ToG_CommonAbstractions.h"
#include "Engine/Canvas.h"
#include "ToG_EnemyPawn_C.generated.h"

class AToG_Pawn_C;

UCLASS()
class TOG_API AToG_EnemyPawn_C : public APawn
{
	GENERATED_BODY()

public:
	//Mesh for this pawn
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* PawnMesh;
	//Mesh for body of this pawn
	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* PawnBody;
	//Mesh for this pawn
	UPROPERTY(EditAnywhere, Category = "Sword Combat Settings")
	UStaticMeshComponent* CameraTargetPoint;
	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Settings")
	ESwordAttackDirection EnemySwordAttackDirection;
	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Settings")
	ESwordAttackType EnemySwordAttackType;
	//Is enemy use sword ability now
	UPROPERTY(EditAnywhere, Category = "Sword Combat Settings")
	bool EnemyOnSwordAction;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Settings")
	bool IsHitByEnemy;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Settings")
	bool IsAttackParried;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Settings")
	ESwordAttackDirection HitDirection;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings")
	float AnimationStartPosition;
	//
	UPROPERTY(EditAnywhere, Category = "Combat Settings")
	AToG_Pawn_C* PlayerPawn;
	//
	float AroundRotationSpeed;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Settings")
	bool CanHit;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Settings")
	ESwordAttackPhase SwordAttackPhase;
	//
	UPROPERTY(EditAnywhere, Category = "Hit Settings")
	float AttackTime;
	//
	float CurrentAttackTime;
	//
	UPROPERTY(EditAnywhere, Category = "Hit Settings")
	double CombatDistance;
	//
	UPROPERTY(EditAnywhere, Category = "Hit Settings")
	double FollowSpeed;
	//
	UPROPERTY(EditAnywhere, Category = "Hit Settings")
	double FollowAttackSpeed;
	//
	double CurrentSpeed;

	// Sets default values for this pawn's properties
	AToG_EnemyPawn_C();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//
	bool HitBySword(FToG_SwordAttack SwordAbility, AToG_Pawn_C* HitPawn);

	//
	void RotatePawnAroundTarget(FVector Start, AActor* Player);

	//
	void FollowTarget(float DeltaTime);

	//
	void AttackTarget(float DeltaTime);
};
