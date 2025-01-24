// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ToG_Pawn_C.h"
#include "ToG_PlayerController_C.generated.h"

UCLASS()
class TOG_API AToG_PlayerController_C : public APlayerController
{
	GENERATED_BODY()

	//Current controlled pawn
	UPROPERTY(EditAnywhere, Category = "Player Components")
	class AToG_Pawn_C* PlayerPawn;

public:
	//Constructor
	AToG_PlayerController_C();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void PlayerTick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	//Call current pawn's move forward/back function
	void TranslateInputForwardBack(const float AxisValue);

	//Call current pawn's move right/left function
	void TranslateInputRightLeft(const float AxisValue);

	//Call current pawn's rotate function
	void TranslateInputMouseX(const float AxisValue);

	//Call current pawn's lookUp function
	void TranslateInputMouseY(const float AxisValue);

	//Call current pawn's combat type function
	void TranslateInputCombatType();
	
	//
    void TranslateInputSwordModePressed();

	//
	void TranslateInputSwordModeReleased();

	//Call current pawn's draw spell function
	void TranslateInputDrawMode();

	//Call current pawn's light attack start processing function
	void TranslateInputActionButtonOnePressed();

	//Call current pawn's light attack end processing function
	void TranslateInputActionButtonOneReleased();

	//Call current pawn's heavy attack start processing function
	void TranslateInputActionButtonTwoPressed();

	//Call current pawn's heavy attack end processing function
	void TranslateInputActionButtonTwoReleased();

	//
	void TranslateInputActionButtonThreePressed();

	//
	void TranslateInputActionButtonThreeReleased();

	//
	void TranslateInputActionButtonFourPressed();

	//
	void TranslateInputActionButtonFourReleased();

	//
	void TranslateInputActionButtonFivePressed();

	//
	void TranslateInputActionButtonFiveReleased();

	//
	void TranslateInputSprintPressed();

	//
	void TranslateInputSprintReleased();

	//
	void TranslateInputSelect(const float AxisValue);
	
};


