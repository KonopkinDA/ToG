// Fill out your copyright notice in the Description page of Project Settings.


#include "ToG_PlayerController_C.h"

#include "GameFramework/InputSettings.h"


AToG_PlayerController_C::AToG_PlayerController_C()
{

}


// Called when the game starts or when spawned
void AToG_PlayerController_C::BeginPlay()
{
	Super::BeginPlay();
	
	//Get current player's pawn
	PlayerPawn = Cast<AToG_Pawn_C>(this->GetPawn());
}

// Called every frame
void AToG_PlayerController_C::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

// Called to bind functionality to input
void AToG_PlayerController_C::SetupInputComponent()
{
	Super::SetupInputComponent();

	/*
	 *  Key bind exchange example
	 *  
	 *	UInputSettings *Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	 *	Settings->RemoveActionMapping(FInputActionKeyMapping("SwordMode", FKey(EKeys::Tab)));
	 *	Settings->AddActionMapping(FInputActionKeyMapping("SwordMode", FKey(EKeys::RightAlt)));
	 *	Settings->SaveKeyMappings();
	*/

	//Create bind's for players input
	//Movement
	InputComponent->BindAxis("MoveForwardBack", this, &AToG_PlayerController_C::TranslateInputForwardBack);
	InputComponent->BindAxis("MoveRightLeft", this, &AToG_PlayerController_C::TranslateInputRightLeft);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputSprintPressed);
	InputComponent->BindAction("Sprint", IE_Released, this, &AToG_PlayerController_C::TranslateInputSprintReleased);
	//Rotation
	InputComponent->BindAxis("Turn", this, &AToG_PlayerController_C::TranslateInputMouseX);
	InputComponent->BindAxis("LookUp", this, &AToG_PlayerController_C::TranslateInputMouseY);
	//Combat
	InputComponent->BindAction("CombatType", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputCombatType);
	InputComponent->BindAction("ActionButtonOne", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputActionButtonOnePressed);
	InputComponent->BindAction("ActionButtonOne", IE_Released, this, &AToG_PlayerController_C::TranslateInputActionButtonOneReleased);
	InputComponent->BindAction("ActionButtonTwo", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputActionButtonTwoPressed);
	InputComponent->BindAction("ActionButtonTwo", IE_Released, this, &AToG_PlayerController_C::TranslateInputActionButtonTwoReleased);
	InputComponent->BindAction("ActionButtonThree", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputActionButtonThreePressed);
	InputComponent->BindAction("ActionButtonThree", IE_Released, this, &AToG_PlayerController_C::TranslateInputActionButtonThreeReleased);
	InputComponent->BindAction("ActionButtonFour", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputActionButtonFourPressed);
	InputComponent->BindAction("ActionButtonFour", IE_Released, this, &AToG_PlayerController_C::TranslateInputActionButtonFourReleased);
	InputComponent->BindAction("ActionButtonFive", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputActionButtonFivePressed);
	InputComponent->BindAction("ActionButtonFive", IE_Released, this, &AToG_PlayerController_C::TranslateInputActionButtonFiveReleased);
	InputComponent->BindAction("CombatMode", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputSwordModePressed);
	InputComponent->BindAction("CombatMode", IE_Released, this, &AToG_PlayerController_C::TranslateInputSwordModeReleased);
	//Spell
	InputComponent->BindAction("DrawSpell", IE_Pressed, this, &AToG_PlayerController_C::TranslateInputDrawMode);
	InputComponent->BindAction("DrawSpell", IE_Released, this, &AToG_PlayerController_C::TranslateInputDrawMode);
	InputComponent->BindAxis("MouseSelect", this, &AToG_PlayerController_C::TranslateInputSelect);
	//Bow
}

//Call current pawn's move forward/back function
void AToG_PlayerController_C::TranslateInputForwardBack(const float AxisValue)
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->MoveCharacterForwardBack(AxisValue);
	}
}

//Call current pawn's move right/left function
void AToG_PlayerController_C::TranslateInputRightLeft(const float AxisValue)
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->MoveCharacterRightLeft(AxisValue);
	}
}

//Call current pawn's rotate function
void AToG_PlayerController_C::TranslateInputMouseX(const float AxisValue)
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->RotateCharacter(AxisValue);
	}
}

//Call current pawn's lookUp function 
void AToG_PlayerController_C::TranslateInputMouseY(const float AxisValue)
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->LookUpCharacter(AxisValue);
	}
}

//Call current pawn's combat type function
void AToG_PlayerController_C::TranslateInputCombatType()
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->ProcessCombatType();
	}
}

//
void AToG_PlayerController_C::TranslateInputSwordModePressed()
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->ProcessCombatModeAction(0);
	}
}

//
void AToG_PlayerController_C::TranslateInputSwordModeReleased()
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->ProcessCombatModeAction(1);
	}
}

//Call current pawn's draw spell function
void AToG_PlayerController_C::TranslateInputDrawMode()
{
	if (PlayerPawn!=nullptr)
	{
		PlayerPawn->ProcessDrawMode();
	}
}

//Call current pawn's light attack start processing function
void AToG_PlayerController_C::TranslateInputActionButtonOnePressed()
{
	if (PlayerPawn!=nullptr)
	{
		//0 is equal to press button
		PlayerPawn->ProcessActionOne(0);
	}
}

//Call current pawn's light attack end processing function
void AToG_PlayerController_C::TranslateInputActionButtonOneReleased()
{
	if (PlayerPawn!=nullptr)
	{
		//1 is equal to release button
		PlayerPawn->ProcessActionOne(1);
	}
}

//Call current pawn's heavy attack start processing function
void AToG_PlayerController_C::TranslateInputActionButtonTwoPressed()
{
	if (PlayerPawn!=nullptr)
	{
		//0 is equal to press button
		PlayerPawn->ProcessActionTwo(0);
	}
}

//Call current pawn's heavy attack end processing function
void AToG_PlayerController_C::TranslateInputActionButtonTwoReleased()
{
	if (PlayerPawn!=nullptr)
	{
		//1 is equal to release button
		PlayerPawn->ProcessActionTwo(1);
	}
}

//
void AToG_PlayerController_C::TranslateInputActionButtonThreePressed()
{
	if (PlayerPawn!=nullptr)
	{
		//0 is equal to press button
		PlayerPawn->ProcessActionThree(0);
	}
}

//
void AToG_PlayerController_C::TranslateInputActionButtonThreeReleased()
{
	if (PlayerPawn!=nullptr)
	{
		//1 is equal to release button
		PlayerPawn->ProcessActionThree(1);
	}
}

//
void AToG_PlayerController_C::TranslateInputActionButtonFourPressed()
{
	if (PlayerPawn!=nullptr)
	{
		//0 is equal to press button
		PlayerPawn->ProcessActionFour(0);
	}
}

//
void AToG_PlayerController_C::TranslateInputActionButtonFourReleased()
{
	if (PlayerPawn!=nullptr)
	{
		//1 is equal to release button
		PlayerPawn->ProcessActionFour(1);
	}
}

//
void AToG_PlayerController_C::TranslateInputActionButtonFivePressed()
{
	if (PlayerPawn!=nullptr)
	{
		//0 is equal to press button
		PlayerPawn->ProcessActionFive(0);
	}
}

//
void AToG_PlayerController_C::TranslateInputActionButtonFiveReleased()
{
	if (PlayerPawn!=nullptr)
	{
		//1 is equal to release button
		PlayerPawn->ProcessActionFive(1);
	}
}

//
void AToG_PlayerController_C::TranslateInputSprintPressed()
{
	if (PlayerPawn!=nullptr)
	{
		//0 is equal to press button
		PlayerPawn->ProcessSprint(0);
	}
}

//
void AToG_PlayerController_C::TranslateInputSprintReleased()
{
	if (PlayerPawn!=nullptr)
	{
		//1 is equal to release button
		PlayerPawn->ProcessSprint(1);
	}
}

//
void AToG_PlayerController_C::TranslateInputSelect(const float AxisValue)
{
	if(AxisValue!=0)
	{
		PlayerPawn->ProcessMouseSelect(AxisValue);
	}
}

	


	

