// Fill out your copyright notice in the Description page of Project Settings.


#include "Tog_Character_C.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATog_Character_C::ATog_Character_C()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATog_Character_C::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATog_Character_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATog_Character_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

