// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h" 
#include "Tog_Character_C.generated.h"

UCLASS()
class TOG_API ATog_Character_C : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATog_Character_C();

	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* FollowCamera;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
