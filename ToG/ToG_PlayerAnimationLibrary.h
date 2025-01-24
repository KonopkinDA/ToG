// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToG_PlayerAnimationLibrary.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOG_API UToG_PlayerAnimationLibrary : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToG_PlayerAnimationLibrary();

	//
	UPROPERTY(EditAnywhere, Category = "BluePrints")
	UClass* SwordCombatAnimClass;
	//
	UPROPERTY(EditAnywhere, Category = "BluePrints")
	UClass* SpellCombatAnimClass;
	//
	UPROPERTY(EditAnywhere, Category = "BluePrints")
	UClass* BowCombatAnimClass;

	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Animations")
	TArray<UAnimSequence*> SwordUpAttackAnimSequences;
	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Animations")
	TArray<UAnimSequence*> SwordLeftAttackAnimSequences;
	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Animations")
	TArray<UAnimSequence*> SwordRightAttackAnimSequences;
	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Animations")
	UAnimSequence* SwordBlockHitAnimSequences;
	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Animations")
	UAnimSequence* SwordBlockRecoveryAnimSequences;
};
