// Fill out your copyright notice in the Description page of Project Settings.


#include "ToG_Pawn_C.h"

#include "CollisionDebugDrawingPublic.h"
#include "FToG_FireBall.h"
#include "FToG_FireMeteor.h"
#include "FToG_SourceLift.h"
#include "FToG_SourcePush.h"
#include "LocalizationDescriptor.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/PointLightComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextBlock.h"

// Sets default values
AToG_Pawn_C::AToG_Pawn_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create static mesh for pawn
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	//
	PawnArmsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterArmsMesh"));
	//
	PawnSkeletalMeshFull = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterSkeletalMeshFull"));
	//
	PawnSkeletalMeshLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterSkeletalMeshLegs"));
	//
	PawnSkeletalMeshArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterSwordArmsSkeletalMesh"));
	//Create camera for pawn
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	//Make the pawn's mesh a root component 
	SetRootComponent(PawnMesh);
	//
	PawnArmsMesh -> SetupAttachment(RootComponent);
	//
	PawnSkeletalMeshFull->SetupAttachment(RootComponent);
	//
	PawnSkeletalMeshLegs->SetupAttachment(RootComponent);
	//Make the pawn's camera a mesh's child
	PawnCamera->SetupAttachment(RootComponent);
	//
	PawnSkeletalMeshArms->SetupAttachment(PawnArmsMesh);
	//
	PawnControlType = EPawnControlType::Free;
	//Set default movement to zero
	Movement = FVector().ZeroVector;
	//Set default rotation to zero
	Rotation = FRotator().ZeroRotator;
	//Set start pawn's and camera's movement and rotation offset to zero
	InputMovement = FVector().ZeroVector;
	InputPawnRotation = FRotator().ZeroRotator;
	InputCameraRotation = FRotator().ZeroRotator;
	//Set default permission of pawn's movement's input component 
	MovementInputPermissions.X = true;
	MovementInputPermissions.Y = true;
	MovementInputPermissions.Z = true;
	//Set default permission of pawn's rotation's input component 
	RotationInputPermissions.X = true;
	RotationInputPermissions.Y = true;
	RotationInputPermissions.Z = true;
	//
	MovementDirection = EMovementDirection::Zero;
	NewMovementDirection = EMovementDirection::Zero;
	LastSideMovementDirection = EMovementDirection::Zero;
	MovementType = EMovementType::Run;
	//Set default speeds of pawn's movements
	WalkSpeed = 250.0f;
	SprintSpeed = 700.0f;
	RunForwardSpeed = 500.0f;
	RunBackSpeed = 300.0f;
	RunSideSpeed = 400.0f;
	RunSideForwardSpeed = 450.0f;
	SwordCombatSpeed = 250.0f;
	CurrentSpeed = 0;
	//
	SwordCombatAttackSlowSpeed = 100.0f;
	//
	StartSpeedCoefficient = 0.5f;
	//
	SpeedAcceleration = 500;
	//
	CurrentSwitchSideMovementTime = 0;
	//
	SwitchSideMovementTime = 0.5f;
	//
	DashPhase = EDashPhase::Zero;
	//
	DashDirection = EMovementDirection::Zero;
	//
	DashSpeed = 700;
	//
	DashTime = 0.15f;
	//
	CurrentDashTime = 0.15f;
	//
	StillSprint = false;
	//Set default speed of pawn's rotation
	TurnSpeed = 50.0f;
	//Set default speed of camera/head lookup
	LookUpSpeed = 50.0f;
	//
	LookUpAngleMax = 75.0f;
	//
	LookUpAngleMin = 75.0f;
	//Set default player's combat type to sword
	PlayerCombatType = EPlayerCombatType::Sword;
	//
	PlayerCombatModeChoosing = EPlayerCombatModeChoosing::Zero;
	//
	TimeToChooseMode = FVector3d(1,1,1);
	//
	CurrentTimeToChooseMode = 0;
	//Set default sword attack direction to nothing
	PlayerSwordAttackDirection = ESwordAttackDirection::Nothing;
	//
	AvailableSwordAttacks = TArray<FToG_SwordAttack>();
	//
	AvailableSwordCombos = TArray<FToG_SwordCombo>();
	//
	LastSwordAttacks = TArray<FToG_SwordAttack>();
	//Set default player's draw mode to disable state
	PlayerDrawMode = EPlayerDrawMode::Off;
	//
	DrawingPoints = TArray<FVector2d>();
	//
	NextPointOffsetSize = 1;
	//
	NextPointOffset = FVector2d().Zero();
	//
	AvailableSpells = TArray<FToG_SpellDrawing>();
	//
	MinDrawingPoints = 5;
	//
	AnimationStartPosition = 0;
	//
	SwordAttackLifeTime = 0.75;
	//
	LastSwordAttackTime = 0;
	//
	SwordAttackPhase = ESwordAttackPhase::Zero;
	//
	SwordBlockPhase = ESwordBlockPhase::Zero;
	//
	IsBlockHold = false;
	//
	SwordAttackAnimationSpeed = 1.0;
	//
	AnimLibrary = CreateDefaultSubobject<UToG_PlayerAnimationLibrary>(TEXT("ToG_PlayerAnimationLibrary"));
	//
	AnimationSequenceStopTime = 0.0;
	//
	SwordLength = 140;

	//
	PlayerSpellStance = EPLayerSpellStance::Source;
	//
	PlayerSpell = nullptr;
	//
	PlayerSpellPhase = ESpellPhase::Zero;
	//
	ShowCombatModeList = false;
	//
	SwordMovementSide = EMovementSide::Zero;
	//
	SwordMovementVector = FVector2d(0,0);
	//
	SwordAttackAnimationSpeedCoefficient = 1;
	//
	SwordNextAttackAllowedTime = 0.1;
}

// Called when the game starts or when spawned
void AToG_Pawn_C::BeginPlay()
{
	Super::BeginPlay();
	//Set pawn's default location and rotation
	SetActorLocation(FVector(1200,350,130));
	SetActorRotation(FRotator(0,-50,0));
	//
	//SwordActor->SetActorHiddenInGame(true);
	//
	BowActor->SetActorHiddenInGame(true);
	//
	SwordActorSocket = PawnSkeletalMeshArms->GetSocketByName("swordsocket");
	//
	BowActorSocket = PawnSkeletalMeshArms->GetSocketByName("bowsocket");
	//
	CollectedMotionSequences();
	//
	CollectedSwordAttacks();
	//
	CollectedSwordCombos();
	//
	CollectedSpells();
	//
	PawnSkeletalMeshLegs->HideBoneByName("spine_01", PBO_None);
	//
	PlayerInterface->AddToViewport();
	//
	SpellModeSockets.Add(PawnSkeletalMeshArms->GetSocketByName("firemodesocket"));
	//
	SpellModeSockets.Add(PawnSkeletalMeshArms->GetSocketByName("groundmodesocket"));
	//
	SpellModeSockets.Add(PawnSkeletalMeshArms->GetSocketByName("electricitymodesocket"));
	//
	SpellModeSockets.Add(PawnSkeletalMeshArms->GetSocketByName("sourcemodesocket"));
	//
	SpellModeSockets.Add(PawnSkeletalMeshArms->GetSocketByName("watermodesocket"));
	//
	SpellModeSockets.Add(PawnSkeletalMeshArms->GetSocketByName("forcemodesocket"));
}

// Called every frame
void AToG_Pawn_C::Tick(float DeltaTime)
{
	//
	Super::Tick(DeltaTime);

	PlayerStateMachine(DeltaTime);

	//
	ShowCurrentCombatText();
	
}

//Determine player's pawn move direction
void AToG_Pawn_C::DetermineCurrentInput(float DeltaTime)
{
	//Choose speed of pawn's moving depending on angle 
	 switch (static_cast<int>(FMath::RadiansToDegrees(FMath::Acos(InputMovement.X / InputMovement.Length()))))
	 {
	 case 0:
	 	NewMovementDirection = EMovementDirection::Forward;
	 	TargetSpeed = RunForwardSpeed;
	 	break;
	 case 45:
	 	if(InputMovement.Y > 0)
	 	{
	 		NewMovementDirection = EMovementDirection::ForwardRight;
	 		TargetSpeed = RunSideForwardSpeed;
	 	}
	 	else
	 	{
	 		NewMovementDirection = EMovementDirection::ForwardLeft;
	 		TargetSpeed = RunSideForwardSpeed;
	 	}
	 	break;
	 case 90:
	 	if(InputMovement.Y > 0)
	 	{
	 		NewMovementDirection = EMovementDirection::Right;
	 		TargetSpeed = RunSideSpeed;
	 	}
	 	else
	 	{
	 		NewMovementDirection = EMovementDirection::Left;
	 		TargetSpeed = RunSideSpeed;
	 	}
	 	break;
	 case 135:
	 	if(InputMovement.Y > 0)
	 	{
	 		NewMovementDirection = EMovementDirection::BackRight;
	 		TargetSpeed = RunBackSpeed;
	 	}
	 	else
	 	{
	 		NewMovementDirection = EMovementDirection::BackLeft;
	 		TargetSpeed = RunBackSpeed;
	 	}
	 	break;
	 case 180:
	 	if(InputMovement.X != 0)
	 	{
	 		NewMovementDirection = EMovementDirection::Back;
	 		TargetSpeed = RunBackSpeed;
	 	}
	 	else
	 	{
	 		NewMovementDirection = EMovementDirection::Zero;
	 		TargetSpeed = 0;
	 	}
	 	break;
	 default:
	 	NewMovementDirection = EMovementDirection::Forward;
	 	TargetSpeed = RunForwardSpeed;
	 	break;
	 }

	if(PlayerCombatType == EPlayerCombatType::Sword && TargetSpeed > SwordCombatAttackSlowSpeed)
	{
		TargetSpeed -= SwordCombatAttackSlowSpeed;
	}

	if((SwordAttackPhase == ESwordAttackPhase::Preparation ||
		PlayerSpellPhase == ESpellPhase::Cast ||
		PlayerSpellPhase == ESpellPhase::Hold) &&
		TargetSpeed > SwordCombatAttackSlowSpeed)
	{
		TargetSpeed -= SwordCombatSpeed;

	}
	
	if(WasMovementSideChanged(MovementDirection, NewMovementDirection, DeltaTime))
	{
		CurrentSpeed = TargetSpeed * StartSpeedCoefficient;
	}
	else
	{
		if(CurrentSpeed == 0)
		{
			CurrentSpeed = TargetSpeed;
		}
	}
	
	if(CurrentSpeed < TargetSpeed)
	{
		if(CurrentSpeed + SpeedAcceleration * DeltaTime < TargetSpeed)
		{
			CurrentSpeed += SpeedAcceleration * DeltaTime;	
		}
		else
		{
			CurrentSpeed = TargetSpeed;
		}
	}
	else
	{
		CurrentSpeed = TargetSpeed;
	}
	
	if(MovementType == EMovementType::Sprint)
	{
		CurrentSpeed += SprintSpeed;
	}

	//
	MovementDirection = NewMovementDirection;
	
	//Add player's input component to total pawn's movement and rotation
	Movement += (GetActorForwardVector() * InputMovement.X + GetActorRightVector() * InputMovement.Y).GetUnsafeNormal() * CurrentSpeed * DeltaTime;
	Rotation += InputPawnRotation * TurnSpeed * DeltaTime;
}

//Change player's input with current permissions
void AToG_Pawn_C::CorrectInputByPermissions()
{
	if(!MovementInputPermissions.X)
	{
		InputMovement.X = 0;
	}
	if(!MovementInputPermissions.Y)
	{
		InputMovement.Y = 0;
	}
	if(!MovementInputPermissions.Z)
	{
		InputMovement.Z = 0;
	}
	
	if(!RotationInputPermissions.X)
	{
		InputPawnRotation.Pitch = 0;
	}
	if(!RotationInputPermissions.Y)
	{
		InputPawnRotation.Yaw = 0;
	}
	if(!RotationInputPermissions.Z)
	{
		InputPawnRotation.Roll = 0;
	}
}

// Called to move player's pawn forward or back
void AToG_Pawn_C::MoveCharacterForwardBack(float AxisValue)
{
	//Add player's input to forward/back movement
	InputMovement.X = AxisValue;
}

// Called to move player's pawn right or left
void AToG_Pawn_C::MoveCharacterRightLeft(float AxisValue)
{
	//Add player's input to right/left movement
	InputMovement.Y = AxisValue;
}

// Called to rotate player's pawn
void AToG_Pawn_C::RotateCharacter(float AxisValue)
{
	// Add player's input to pawn's rotation
	InputPawnRotation.Yaw = AxisValue;
}

// Called to rotate player's camera
void AToG_Pawn_C::LookUpCharacter(float AxisValue)
{
	// Add player's input to camera's rotation
	InputCameraRotation.Pitch = AxisValue;
}

// Need to be replaced
void AToG_Pawn_C::RotateCamera(float DeltaTime)
{
	//Check if camera have permission to lookup
	if(RotationInputPermissions.X)
	{
		//Create empty rotator
		FRotator NewRotation = FRotator().ZeroRotator;
		//Modify rotation with player's input
		NewRotation.Pitch = InputCameraRotation.Pitch * LookUpSpeed * DeltaTime;
		//Add new rotation to the pawn's camera
		if(const float NextAngle = PawnCamera->GetRelativeRotation().Pitch + NewRotation.Pitch; NextAngle > LookUpAngleMax)
		{
			NewRotation.Pitch = LookUpAngleMax - PawnCamera->GetRelativeRotation().Pitch;
			CameraRotation += NewRotation;
		}
		else
		{
			if(NextAngle < (-1) * LookUpAngleMin)
			{
				NewRotation.Pitch = (-1) * LookUpAngleMin - PawnCamera->GetRelativeRotation().Pitch;
				CameraRotation += NewRotation;
			}
			else
			{
				CameraRotation += NewRotation;
			}
		}
	}
}

//Called to change current combat type
void AToG_Pawn_C::ProcessCombatType()
{
	if (PawnControlType == EPawnControlType::Free)
	{
		switch (PlayerCombatType)
		{
		case EPlayerCombatType::Zero:
			break;
		case EPlayerCombatType::Sword:
			PlayerCombatType = EPlayerCombatType::Bow;
			BowActor->SetActorHiddenInGame(false);
			SwordActor->SetActorHiddenInGame(true);
			PawnSkeletalMeshArms->SetAnimClass(AnimLibrary->BowCombatAnimClass);
			break;
		case EPlayerCombatType::Bow:
			PlayerCombatType = EPlayerCombatType::Spell;
			BowActor->SetActorHiddenInGame(true);
			SwordActor->SetActorHiddenInGame(true);
			PawnSkeletalMeshArms->SetAnimClass(AnimLibrary->SpellCombatAnimClass);
			PlayerSpellStance = EPLayerSpellStance::Fire;
			break;
		case EPlayerCombatType::Spell:
			PlayerCombatType = EPlayerCombatType::Sword;
			PlayerSpellStance = EPLayerSpellStance::Source;
			BowActor->SetActorHiddenInGame(true);
			SwordActor->SetActorHiddenInGame(false);
			PawnSkeletalMeshArms->SetAnimClass(AnimLibrary->SwordCombatAnimClass);
			break;
		}
	}
}

//
void AToG_Pawn_C::ProcessCombatModeAction(int Type)
{
	switch (Type)
	{
	case 0:
		ShowCombatModeList = true;
		break;
	case 1:
		ShowCombatModeList = false;
		break;
	}
}

//
void AToG_Pawn_C::ProcessMouseSelect(float AxisValue)
{
	switch (PlayerCombatType)
	{
	case EPlayerCombatType::Zero:
		break;
	case EPlayerCombatType::Sword:
		break;
	case EPlayerCombatType::Spell:
		switch (PlayerCombatModeChoosing)
		{
	case  EPlayerCombatModeChoosing::Zero:
		break;
	case EPlayerCombatModeChoosing::Show:
		break;
	case EPlayerCombatModeChoosing::Hold:
		switch (PlayerSpellStance)
		{
	case EPLayerSpellStance::Fire:
		if(AxisValue < 0)
		{
			PlayerSpellStance = EPLayerSpellStance::Ground;
			Cast<UPointLightComponent>(SpellModeActors[1]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[0]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		else
		{
			PlayerSpellStance = EPLayerSpellStance::Force;
			Cast<UPointLightComponent>(SpellModeActors[5]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[0]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		break;
	case EPLayerSpellStance::Ground:
		if(AxisValue < 0)
		{
			PlayerSpellStance = EPLayerSpellStance::Electricity;
			Cast<UPointLightComponent>(SpellModeActors[2]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[1]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		else
		{
			PlayerSpellStance = EPLayerSpellStance::Fire;
			Cast<UPointLightComponent>(SpellModeActors[0]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[1]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		break;
	case EPLayerSpellStance::Electricity:
		if(AxisValue < 0)
		{
			PlayerSpellStance = EPLayerSpellStance::Source;
			Cast<UPointLightComponent>(SpellModeActors[3]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[2]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		else
		{
			PlayerSpellStance = EPLayerSpellStance::Ground;
			Cast<UPointLightComponent>(SpellModeActors[1]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[2]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		break;
	case EPLayerSpellStance::Source:
		if(AxisValue < 0)
		{
			PlayerSpellStance = EPLayerSpellStance::Water;
			Cast<UPointLightComponent>(SpellModeActors[4]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[3]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		else
		{
			PlayerSpellStance = EPLayerSpellStance::Electricity;
			Cast<UPointLightComponent>(SpellModeActors[2]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[3]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		break;
	case EPLayerSpellStance::Water:
		if(AxisValue < 0)
		{
			PlayerSpellStance = EPLayerSpellStance::Force;
			Cast<UPointLightComponent>(SpellModeActors[5]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[4]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		else
		{
			PlayerSpellStance = EPLayerSpellStance::Source;
			Cast<UPointLightComponent>(SpellModeActors[3]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[4]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		break;
	case EPLayerSpellStance::Force:
		if(AxisValue < 0)
		{
			PlayerSpellStance = EPLayerSpellStance::Fire;
			Cast<UPointLightComponent>(SpellModeActors[0]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[5]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		else
		{
			PlayerSpellStance = EPLayerSpellStance::Water;
			Cast<UPointLightComponent>(SpellModeActors[4]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
			Cast<UPointLightComponent>(SpellModeActors[5]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
		}
		break;
		}
		break;
	case EPlayerCombatModeChoosing::Hide:
		break;
		}
		break;
	case EPlayerCombatType::Bow:
		break;
	}
}

//
void AToG_Pawn_C::CombatModeActionUpdate(float DeltaTime)
{
	switch (PlayerCombatModeChoosing)
	{
	case EPlayerCombatModeChoosing::Zero:
		if(ShowCombatModeList)
		{
			CurrentTimeToChooseMode += DeltaTime;
			if(CurrentTimeToChooseMode >= TimeToChooseMode[0])
			{
				PlayerCombatModeChoosing = EPlayerCombatModeChoosing::Show;
				CurrentTimeToChooseMode = 0;
			}
		}
		else
		{
			//Если кнопку отпустили и при этом меню выбора не начало открываться, но кнопка была зажата хоть
			//какое-то количество времени - используем быстрое переключение
			if(CurrentTimeToChooseMode != 0)
			{
				switch (PlayerCombatType)
				{
				case EPlayerCombatType::Sword:
					break;
				case EPlayerCombatType::Spell:
					switch (PlayerSpellStance)
					{
				case EPLayerSpellStance::Fire:
					PlayerSpellStance = EPLayerSpellStance::Ground;
						Cast<UPointLightComponent>(SpellModeActors[1]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
						Cast<UPointLightComponent>(SpellModeActors[0]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
						break;
				case EPLayerSpellStance::Ground:
					PlayerSpellStance = EPLayerSpellStance::Electricity;
						Cast<UPointLightComponent>(SpellModeActors[2]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
						Cast<UPointLightComponent>(SpellModeActors[1]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
						break;
				case EPLayerSpellStance::Electricity:
					PlayerSpellStance = EPLayerSpellStance::Source;
						Cast<UPointLightComponent>(SpellModeActors[3]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
						Cast<UPointLightComponent>(SpellModeActors[2]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
						break;
				case EPLayerSpellStance::Source:
					PlayerSpellStance = EPLayerSpellStance::Water;
						Cast<UPointLightComponent>(SpellModeActors[4]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
						Cast<UPointLightComponent>(SpellModeActors[3]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
						break;
				case EPLayerSpellStance::Water:
					PlayerSpellStance = EPLayerSpellStance::Force;
						Cast<UPointLightComponent>(SpellModeActors[5]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
						Cast<UPointLightComponent>(SpellModeActors[4]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
						break;
				case EPLayerSpellStance::Force:
					PlayerSpellStance = EPLayerSpellStance::Fire;
						Cast<UPointLightComponent>(SpellModeActors[0]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(5000);
						Cast<UPointLightComponent>(SpellModeActors[5]->GetComponentByClass(UPointLightComponent::StaticClass()))->SetIntensity(0);
					break;
					}
					break;
				case EPlayerCombatType::Bow:
					break;
				}
			}
			CurrentTimeToChooseMode = 0;
		}
		break;
	case EPlayerCombatModeChoosing::Show:
		if(ShowCombatModeList)
		{
			CurrentTimeToChooseMode += DeltaTime;
			if (CurrentTimeToChooseMode >= TimeToChooseMode[1])
			{
				PlayerCombatModeChoosing = EPlayerCombatModeChoosing::Hold;
				CurrentTimeToChooseMode = 0;
			}
		}
		else
		{
			PlayerCombatModeChoosing = EPlayerCombatModeChoosing::Hide;
			CurrentTimeToChooseMode = TimeToChooseMode[2] - CurrentTimeToChooseMode; // Справедливо только пока время фаз одинаково
		}
		break;
	case EPlayerCombatModeChoosing::Hold:
		if(!ShowCombatModeList)
		{
			PlayerCombatModeChoosing = EPlayerCombatModeChoosing::Hide;
			CurrentTimeToChooseMode = 0;
		}
		break;
	case EPlayerCombatModeChoosing::Hide:
		if(ShowCombatModeList)
		{
			PlayerCombatModeChoosing = EPlayerCombatModeChoosing::Show;
			CurrentTimeToChooseMode = TimeToChooseMode[2] - CurrentTimeToChooseMode; //
		}
		else
		{
			CurrentTimeToChooseMode += DeltaTime;
			if(CurrentTimeToChooseMode >= TimeToChooseMode[2])
			{
				PlayerCombatModeChoosing = EPlayerCombatModeChoosing::Zero;
				CurrentTimeToChooseMode = 0;
			}
		}
		break;
	}
}

//Called to process draw spell mode
void AToG_Pawn_C::ProcessDrawMode()
{
	if(PawnControlType==EPawnControlType::Free)
	{
		if(PlayerCombatType == EPlayerCombatType::Spell)
		{
			switch (PlayerDrawMode)
			{
			case EPlayerDrawMode::On:
				{
					//
					DrawRayShape(CorrelateDrawingToSpell(AvailableSpells).DrawingPoints);
					//
					PlayerDrawMode = EPlayerDrawMode::Off;
					//Deny permissions for pawn's and camera turning by player's input
					RotationInputPermissions.Y = true;
					RotationInputPermissions.X = true;
					TurnSpeed = 150.0f;
			
					DrawingPoints.Empty();
					NextPointOffset = FVector2d().Zero();
				}
				break;
			case EPlayerDrawMode::Off:
				{
					PlayerDrawMode = EPlayerDrawMode::On;
					//Deny permissions for pawn's and camera turning by player's input
					RotationInputPermissions.Y = false;
					RotationInputPermissions.X = false;
					TurnSpeed = 0.0f;
			
					DrawingPoints.Add(FVector2d().Zero());
				}
				break;
			}
		}
	}
}

//Terminate player's drawing points
void AToG_Pawn_C::DrawSpell(float DeltaTime)
{
	if(NextPointOffset.Length() * DeltaTime * 100 < NextPointOffsetSize)
		{
			NextPointOffset.X += InputCameraRotation.Pitch * DeltaTime * 100;
			NextPointOffset.Y += InputPawnRotation.Yaw * DeltaTime * 100;
		}
	else
		{
			NextPointOffset = FVector2d(NextPointOffset.X,NextPointOffset.Y).GetSafeNormal() * NextPointOffsetSize + DrawingPoints.Last();
			DrawingPoints.Add(NextPointOffset);
			NextPointOffset = FVector2d().Zero();
		}
}

//Temp rays for random shape drawing in editor
void AToG_Pawn_C::DrawRayShape(TArray<FVector2d> Shape)
{
	for (int i = 1; i < Shape.Num(); i++)
	{
		DrawDebugLine(
		GetWorld(),
	PawnCamera -> GetComponentLocation() + PawnCamera -> GetForwardVector() * 40
	+ PawnCamera -> GetRightVector() * Shape[i].Y +
	PawnCamera -> GetUpVector() * Shape[i].X,
	PawnCamera -> GetComponentLocation() + PawnCamera -> GetForwardVector() * 40
	+ PawnCamera -> GetRightVector() * Shape[i-1].Y +
	PawnCamera -> GetUpVector() * Shape[i-1].X,
	FColor::Red,
	false,
	150.0f,
	0.0f,
	0.5f
	);
	}
}

//Calculate difference between two shapes
double AToG_Pawn_C::CalculateShapesDifference(TArray<FVector2d> Spell, TArray<FVector2d> Drawing)
{
	double Difference = 0;
	int k = 1;
	FVector2d SpellVector;
	FVector2d DrawingVector;

	for(int i=1; i < Spell.Num(); i++)
	{
		SpellVector = Spell[i] - Spell[i-1];
		DrawingVector = FVector2d().Zero();
		for(int j = k; (double)j/(Drawing.Num()-1) <= (double)i/(Spell.Num()-1) && j < Drawing.Num(); j++, k++)
		{
			DrawingVector += Drawing[j] - Drawing[j-1];
		}
		DrawingVector *= SpellVector.Length() / DrawingVector.Length();
		Difference += (SpellVector - DrawingVector).Length();
	}
	
	return Difference;
}

//Find correlation coefficient between spells
FToG_SpellDrawing AToG_Pawn_C::CorrelateDrawingToSpell(TArray<FToG_SpellDrawing> Spells)
{
	FString SpellName = "Nothing";
	double MinDifference = 100;
	int SpellIndex = 0;

	if(DrawingPoints.Num() < MinDrawingPoints)
	{
		return FToG_SpellDrawing("Nothing", TArray<FVector2d>());
	}
	
	for(int i = 0; i < Spells.Num(); i++)
	{
		double Difference;
		if(Spells[i].DrawingPoints.Num() < DrawingPoints.Num())
		{
			Difference = CalculateShapesDifference(Spells[i].DrawingPoints, DrawingPoints);
		}
		else
		{
			Difference = CalculateShapesDifference(DrawingPoints, Spells[i].DrawingPoints);
		}
		if(Difference < MinDifference)
		{
			MinDifference = Difference;
			SpellIndex = i;
		}
	}
	if(MinDifference <= Spells[SpellIndex].MaxDifference)
	{
		DrawRayShape(Spells[SpellIndex].DrawingPoints);
		return Spells[SpellIndex];
	}
	else
	{
		return FToG_SpellDrawing("Nothing", TArray<FVector2d>());
	}
}

//
void AToG_Pawn_C::ProcessActionOne(int Type)
{
	if(PawnControlType == EPawnControlType::Free)
	{
		switch (PlayerCombatType)
		{
		case EPlayerCombatType::Sword:
			{
				switch (Type)
				{
				case 0:
					{
						DetermineSwordMovementSide();
						switch (SwordAttackPhase)
						{
						case ESwordAttackPhase::Zero:
							//Need to check for action keys
							if(!SwordBlock)
							{
								//
								ChangeSwordAttack(0);
								SwordAttackPhase = ESwordAttackPhase::Preparation;
							}
							else
							{
								if(SwordBlockPhase == ESwordBlockPhase::Unblock)
								{
									//
									ChangeSwordAttack(0);
									SwordAttackPhase = ESwordAttackPhase::Preparation;
									//
									SwordBlock = nullptr;
									SwordBlockPhase = ESwordBlockPhase::Zero;
								}
							}
							break;
						case ESwordAttackPhase::Preparation:
							break;
						case ESwordAttackPhase::Attack:
							if(SwordAttack->Duration.Y < SwordNextAttackAllowedTime)
							{
								// if(SwordAttack->AnimationId == AvailableSwordAttacks[0].AnimationId)
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[1]));
								// }
								// else
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[0]));
								// }
								NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[0]));
								//
								NextSwordAttack->FullDuration*=SwordAttackAnimationSpeedCoefficient;
								NextSwordAttack->Duration*=SwordAttackAnimationSpeedCoefficient;
								//
							}
							break;
						case ESwordAttackPhase::Returning:
							// if(SwordAttack -> AnimationId == AvailableSwordAttacks[0].AnimationId)
							// {
							// 	ChangeSwordAttack(1);
							// }
							// else
							// {
							// 	ChangeSwordAttack(0);
							// }
							ChangeSwordAttack(0);
							SwordAttackPhase = ESwordAttackPhase::Preparation;
							break;
						case ESwordAttackPhase::Rebound:
							if(SwordAttack->Duration.W < SwordNextAttackAllowedTime * 2)
							{
								// if(SwordAttack->AnimationId == AvailableSwordAttacks[1].AnimationId)
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[1]));
								// }
								// else
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[0]));
								// }
								NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[0]));
								//
								NextSwordAttack->FullDuration*=SwordAttackAnimationSpeedCoefficient;
								NextSwordAttack->Duration*=SwordAttackAnimationSpeedCoefficient;
								//
							}
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Spell:
			{
				switch (Type)
				{
				case 0:
					{
						DetermineSpell(0,1);
					}
					break;
				case 1:
					{
						DetermineSpell(1,1);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Bow:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}
}

//
void AToG_Pawn_C::ProcessActionTwo(int Type)
{
	if(PawnControlType == EPawnControlType::Free)
	{
		switch (PlayerCombatType)
		{
		case EPlayerCombatType::Sword:
			{
				switch (Type)
				{
				case 0:
					{
						DetermineSwordMovementSide();
						switch (SwordAttackPhase)
						{
						case ESwordAttackPhase::Zero:
							//Need to check for action keys
							if(!SwordBlock)
							{
								//
								ChangeSwordAttack(1);
								SwordAttackPhase = ESwordAttackPhase::Preparation;
							}
							else
							{
								if(SwordBlockPhase == ESwordBlockPhase::Unblock)
								{
									//
									ChangeSwordAttack(1);
									SwordAttackPhase = ESwordAttackPhase::Preparation;
									//
									SwordBlock = nullptr;
									SwordBlockPhase = ESwordBlockPhase::Zero;
								}
							}
							break;
						case ESwordAttackPhase::Preparation:
							break;
						case ESwordAttackPhase::Attack:
							if(SwordAttack->Duration.Y < SwordNextAttackAllowedTime)
							{
								// if(SwordAttack->AnimationId == AvailableSwordAttacks[0].AnimationId)
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[1]));
								// }
								// else
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[0]));
								// }
								NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[1]));
								NextSwordAttack->Duration *= SwordAttackAnimationSpeedCoefficient;
								NextSwordAttack->FullDuration *= SwordAttackAnimationSpeedCoefficient;
							}
							break;
						case ESwordAttackPhase::Returning:
							// if(SwordAttack -> AnimationId == AvailableSwordAttacks[0].AnimationId)
							// {
							// 	ChangeSwordAttack(1);
							// }
							// else
							// {
							// 	ChangeSwordAttack(0);
							// }
							ChangeSwordAttack(1);
							SwordAttackPhase = ESwordAttackPhase::Preparation;
							break;
						case ESwordAttackPhase::Rebound:
							if(SwordAttack->Duration.W < SwordNextAttackAllowedTime * 2)
							{
								// if(SwordAttack->AnimationId == AvailableSwordAttacks[1].AnimationId)
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[1]));
								// }
								// else
								// {
								// 	NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[0]));
								// }
								NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[1]));
								NextSwordAttack->Duration *= SwordAttackAnimationSpeedCoefficient;
								NextSwordAttack->FullDuration *= SwordAttackAnimationSpeedCoefficient;
							}
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Spell:
			{
				switch (Type)
				{
				case 0:
					{
						DetermineSpell(0,1);
					}
					break;
				case 1:
					{
						DetermineSpell(1,1);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Bow:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}
}

//
void AToG_Pawn_C::ProcessActionTwo_OldBlock(int Type)
{
	if(PawnControlType == EPawnControlType::Free)
	{
		switch (PlayerCombatType)
		{
		case EPlayerCombatType::Sword:
			{
				switch (Type)
				{
				case 0:
					{
						switch (SwordAttackPhase)
						{
						case ESwordAttackPhase::Zero:
							SwordBlockPhase = ESwordBlockPhase::Block;
							IsBlockHold = true;
							SwordBlock = new FToG_SwordBlock("Block", FVector4d(0.25,0.25,0.12,0.38));
							break;
						case ESwordAttackPhase::Preparation:
							break;
						case ESwordAttackPhase::Attack:
							break;
						case ESwordAttackPhase::Returning:
							//
							SwordAttackPhase = ESwordAttackPhase::Zero;
							SwordAttack = nullptr;
							NextSwordAttack = nullptr;
							SwordAttackId = 0;
							//
							SwordBlockPhase = ESwordBlockPhase::Block;
							IsBlockHold = true;
							SwordBlock = new FToG_SwordBlock("Block", FVector4d(0.25,0.25,0.12,0.38));
							break;
						}
					}
					break;
				case 1:
					{
						IsBlockHold = false;
						switch (SwordBlockPhase)
						{
						case ESwordBlockPhase::Zero:
							break;
						case ESwordBlockPhase::Block:
							break;
						case ESwordBlockPhase::Hold:
							SwordBlockPhase = ESwordBlockPhase::Unblock;
							break;
						case ESwordBlockPhase::Unblock:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Spell:
			{
				switch (Type)
				{
				case 0:
					{
						DetermineSpell(0,2);
					}
					break;
				case 1:
					{
						DetermineSpell(1,2);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Bow:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}
}

//
void AToG_Pawn_C::ProcessActionThree(int Type)
{
	if(PawnControlType == EPawnControlType::Free)
	{
		switch (PlayerCombatType)
		{
		case EPlayerCombatType::Sword:
			{
				switch (Type)
				{
				case 0:
					{
						DetermineSwordMovementSide();
						switch (SwordAttackPhase)
						{
						case ESwordAttackPhase::Zero:
							//Need to check for action keys
							if(!SwordBlock)
							{
								//
								ChangeSwordAttack(2);
								SwordAttackPhase = ESwordAttackPhase::Preparation;	
							}
							else
							{
								if(SwordBlockPhase == ESwordBlockPhase::Unblock)
								{
									//
									ChangeSwordAttack(2);
									SwordAttackPhase = ESwordAttackPhase::Preparation;
									//
									SwordBlock = nullptr;
									SwordBlockPhase = ESwordBlockPhase::Zero;
								}
							}
							break;
						case ESwordAttackPhase::Preparation:
							break;
						case ESwordAttackPhase::Attack:
							// if(SwordAttack->AnimationId != AvailableSwordAttacks[2].AnimationId)
							// {
							// 	if(SwordAttack->Duration.Y < 0.1)
							// 	{
							// 		NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[2]));
							// 	}	
							// }
							if(SwordAttack->Duration.Y < SwordNextAttackAllowedTime)
							{
								NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[2]));
								NextSwordAttack->Duration *= SwordAttackAnimationSpeedCoefficient;
								NextSwordAttack->FullDuration *= SwordAttackAnimationSpeedCoefficient;
							}	
							break;
						case ESwordAttackPhase::Returning:
							ChangeSwordAttack(2);
							SwordAttackPhase = ESwordAttackPhase::Preparation;
							break;
						case ESwordAttackPhase::Rebound:
							if(SwordAttack->Duration.W < SwordNextAttackAllowedTime * 2)
							{
								NextSwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[2]));
								NextSwordAttack->Duration *= SwordAttackAnimationSpeedCoefficient;
								NextSwordAttack->FullDuration *= SwordAttackAnimationSpeedCoefficient;
							}	
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Spell:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Bow:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}
}

//
void AToG_Pawn_C::ProcessActionFour(int Type)
{
	if(PawnControlType == EPawnControlType::Free)
	{
		switch (PlayerCombatType)
		{
		case EPlayerCombatType::Sword:
			{
				switch (Type)
				{
				case 0:
					{
						//Temp
						ProcessHit();
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Spell:
			{
				switch (Type)
				{
				case 0:
					DetermineSpell(0,4);
					break;
				case 1:
					DetermineSpell(1, 4);
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Bow:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}
}

//
void AToG_Pawn_C::ProcessActionFive(int Type)
{
	if(PawnControlType == EPawnControlType::Free)
	{
		switch (PlayerCombatType)
		{
		case EPlayerCombatType::Sword:
			{
				switch (Type)
				{
				case 0:
					{

					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Spell:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		case EPlayerCombatType::Bow:
			{
				switch (Type)
				{
				case 0:
					{
						
					}
					break;
				case 1:
					{
						
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}
}

//Check if last few sword abilities can assamble in combo
FToG_SwordAttack AToG_Pawn_C::CheckForCombo(FToG_SwordAttack NewSwordAbility)
{
	if(LastSwordAttacks.Num() < 5)
	{
		LastSwordAttacks.Add(NewSwordAbility);
		LastSwordAttackTime = SwordAttackLifeTime;
	}
	else
	{
		LastSwordAttacks.RemoveAt(0);
		LastSwordAttacks.Add(NewSwordAbility);
		LastSwordAttackTime = SwordAttackLifeTime;
	}

	for (int i = 0; i < AvailableSwordCombos.Num(); i++)
	{
		for(int j = 0; j < AvailableSwordCombos[i].RequiredAbilities.Num() && j < LastSwordAttacks.Num(); j++)
		{
			if(LastSwordAttacks.Num() < AvailableSwordCombos[i].RequiredAbilities.Num())
			{
				break;
			}
			if(AvailableSwordCombos[i].RequiredAbilities[AvailableSwordCombos[i].RequiredAbilities.Num() - 1 - j] != LastSwordAttacks[LastSwordAttacks.Num() -1 - j].Name)
			{
				break;
			}
			if(j == AvailableSwordCombos[i].RequiredAbilities.Num()-1)
			{
				LastSwordAttacks.RemoveAt(LastSwordAttacks.Num() - 1);
				LastSwordAttacks.Add(AvailableSwordCombos[i].Ability);
				LastSwordAttackTime = SwordAttackLifeTime;
				return AvailableSwordCombos[i].Ability;
			}
		}
	}
	return NewSwordAbility;
}

//
void AToG_Pawn_C::DetermineNewPawnLocation()
{
	SetActorLocation(GetActorLocation() + Movement);
}

//
void AToG_Pawn_C::DetermineNewPawnRotation()
{
	SetActorRotation(GetActorRotation() + Rotation);
}

//
void AToG_Pawn_C::DetermineNewCameraRotation()
{
	PawnCamera -> SetRelativeRotation(PawnCamera->GetRelativeRotation() + CameraRotation);
}

//
void AToG_Pawn_C::NullifyMovementAndRotation()
{
	Movement = FVector().ZeroVector;
	Rotation = FRotator().ZeroRotator;
	CameraRotation = FRotator().ZeroRotator;
}

//
void AToG_Pawn_C::PlayerStateMachine(float DeltaTime)
{
	switch (PawnControlType)
	{
	case EPawnControlType::Free:
		//
		ProcessSwordAbilities(DeltaTime);
	
		//Determine speed for current move direction
		DetermineCurrentInput(DeltaTime);

		//
		ProcessDash(DeltaTime);

		//Need to be replaced to camera class
		RotateCamera(DeltaTime);

		//Rotate arms to camera new rotation
		ArmsCameraRotation(DeltaTime);
	
		//Change pawn's position with new movement
		DetermineNewPawnLocation();

		//Change pawn's rotation
		DetermineNewPawnRotation();

		//Change pawn's camera rotation
		DetermineNewCameraRotation();

		//Clean used sword abilities array if enough time has passed
		LastSwordAbilitiesClean(DeltaTime);

		//Set movement and rotation to zero after pawn's move and rotate
		NullifyMovementAndRotation();

		//Move sword mesh to new socket location
		MoveSwordActorToSocket(DeltaTime);
		
		//
		ProcessSpell(DeltaTime);

		//
		CombatModeActionUpdate(DeltaTime);

		//
		if(PlayerDrawMode == EPlayerDrawMode::On)
		{
			//
			DrawRaySpell();
			
			//
			DrawSpell(DeltaTime);
		}

		break;
		
	case EPawnControlType::Animated:
		//
		//ProcessSwordAbilities(DeltaTime);
		break;
	}
}

//
void AToG_Pawn_C::ProcessSwordAbilities(float DeltaTime)
{
	if(SwordAttack)
	{
		switch (SwordAttackPhase)
		{
		case ESwordAttackPhase::Zero :
			break;
		case ESwordAttackPhase::Preparation :
			if(SwordAttack -> Duration.X > 0)
			{
				SwordAttack -> Duration.X -= DeltaTime;
			}
			else
			{
				SwordAttackPhase = ESwordAttackPhase::Attack;
				//
				SwordAttackAnimationSpeed = SwordAttackAnimationSequence->GetTimeAtFrame(
					SwordAttackAnimationSequence->GetNumberOfFrames()) / SwordAttack->FullDuration.Y;
			}
			break;
		case ESwordAttackPhase::Attack :
			if(SwordAttack -> Duration.Y > 0)
			{
				//Temp solution
				MoveSwordActorToSocket(DeltaTime);
				
				SwordAttack -> Duration.Y -= DeltaTime;

				//
				ProcessSwordHits(DeltaTime);
			}
			else
			{
				if(NextSwordAttack)
				{
					SwordAttack = NextSwordAttack;
					SwordAttackId = SwordAttack->AnimationId;
					NextSwordAttack = nullptr;
					SwordAttackAnimationSequence = SwordAttack->AnimationSequences[0];
					SwordAttackPhase = ESwordAttackPhase::Preparation;
				}
				else
				{
					SwordAttackPhase = ESwordAttackPhase::Returning;
				}
			}
			break;
		case ESwordAttackPhase::Returning :
			if(SwordAttack -> Duration.Z > 0)
			{
				SwordAttack -> Duration.Z -= DeltaTime;
			}
			else
			{
				SwordAttackPhase = ESwordAttackPhase::Zero;
				SwordAttack = nullptr;
				SwordAttackId = 0;
			}
			break;
		case  ESwordAttackPhase::Rebound :
			if(SwordAttack -> Duration.W > 0)
			{
				SwordAttack -> Duration.W -= DeltaTime;
			}
			else
			{
				if(!NextSwordAttack)
				{
					SwordAttackPhase = ESwordAttackPhase::Zero;
					SwordAttack = nullptr;
					SwordAttackId = 0;	
				}
				else
				{
					SwordAttack = NextSwordAttack;
					SwordAttackId = SwordAttack->AnimationId;
					NextSwordAttack = nullptr;
					SwordAttackAnimationSequence = SwordAttack->AnimationSequences[0];
					SwordAttackPhase = ESwordAttackPhase::Preparation;
				}
			}
			break;
		}
	}
	else
	{
		if(SwordBlock)
		{
			switch (SwordBlockPhase)
			{
			case ESwordBlockPhase::Zero:
				break;
			case ESwordBlockPhase::Block:
				if(SwordBlock -> Duration.X > 0)
				{
					SwordBlock -> Duration.X -= DeltaTime;
				}
				else
				{
					SwordBlockPhase = ESwordBlockPhase::Hold;
				}
				break;
			case ESwordBlockPhase::Hold:
				if(!IsBlockHold)
				{
					SwordBlockPhase = ESwordBlockPhase::Unblock;
				}
				break;
			case ESwordBlockPhase::Unblock:
				if(SwordBlock -> Duration.Y > 0)
				{
					SwordBlock -> Duration.Y -= DeltaTime;
				}
				else
				{
					SwordBlockPhase = ESwordBlockPhase::Zero;
					SwordBlock = nullptr;
				}
				break;
			case ESwordBlockPhase::Hit:
				if(SwordBlock -> Duration.Z > 0)
				{
					SwordBlock -> Duration.Z -= DeltaTime;
				}
				else
				{
					SwordBlockPhase = ESwordBlockPhase::Recovery;
					SwordBlock -> Duration.Z = SwordBlock -> FullDuration.Z;
				}
				break;
			case ESwordBlockPhase::Recovery:
				if(SwordBlock -> Duration.W > 0)
				{
					SwordBlock -> Duration.W -= DeltaTime;
				}
				else
				{
					SwordBlockPhase = ESwordBlockPhase::Hold;
					SwordBlock -> Duration.W = SwordBlock -> FullDuration.W;
					AnimationSequenceStopTime = 0;
				}
				break;
			}
		}
	}
}

//
void AToG_Pawn_C::LastSwordAbilitiesClean(float DeltaTime)
{
	if(LastSwordAttackTime > 0)
	{
		LastSwordAttackTime -= DeltaTime;
	}
	else
	{
		LastSwordAttacks.Empty();
		LastSwordAttackTime = 0;
	}
}

//
void AToG_Pawn_C::MoveSwordActorToSocket(float DeltaTime)
{
	SwordActor->SetActorLocation(SwordActorSocket->GetSocketTransform(PawnSkeletalMeshArms).GetLocation());
	SwordActor->SetActorRotation(SwordActorSocket->GetSocketTransform(PawnSkeletalMeshArms).GetRotation());

	BowActor->SetActorLocation(BowActorSocket->GetSocketTransform(PawnSkeletalMeshArms).GetLocation());
	BowActor->SetActorRotation(BowActorSocket->GetSocketTransform(PawnSkeletalMeshArms).GetRotation());

	for (int i = 0; i < SpellModeActors.Num(); i++)
	{
		SpellModeActors[i]->SetActorLocation(SpellModeSockets[i]->GetSocketTransform(PawnSkeletalMeshArms).GetLocation());
		SpellModeActors[i]->SetActorRotation(SpellModeSockets[i]->GetSocketTransform(PawnSkeletalMeshArms).GetRotation());
	}
}

//
FTransform AToG_Pawn_C::FindBoneNewTransform(UAnimSequence* PawnAnimation, FName BoneName, FTransform BoneRoot, double NextFrameTime)
{
	FTransform BoneOffset;
	FSkeletonPoseBoneIndex BoneIndex = FSkeletonPoseBoneIndex(PawnSkeletalMeshArms->GetBoneIndex(BoneName));
	PawnAnimation->GetBoneTransform(BoneOffset, BoneIndex, NextFrameTime,false);

	FVector BoneLocation = BoneRoot.TransformPosition(BoneOffset.GetLocation());
	UE::Math::TQuat BoneRotation = BoneRoot.TransformRotation(BoneOffset.GetRotation());
	FTransform BoneTransform = FTransform(BoneRotation, BoneLocation, FVector(1,1,1));

	return BoneTransform;
}

//
FTransform AToG_Pawn_C::FindBoneNewTransformRecurs(UAnimSequence* PawnAnimation, FName BoneName, double NextFrameTime)
{
	if(BoneName == "root")
	{
		return PawnSkeletalMeshArms->GetBoneTransform(PawnSkeletalMeshArms->GetBoneIndex("root"));
	}
	else
	{
		FName ParentBoneName = PawnSkeletalMeshArms->GetParentBone(BoneName);
		FTransform ParentBoneTransform = FindBoneNewTransformRecurs(PawnAnimation, ParentBoneName, NextFrameTime);
		return FindBoneNewTransform(PawnAnimation, BoneName, ParentBoneTransform, NextFrameTime);
	}
}

//
void AToG_Pawn_C::ProcessSwordHits(float DeltaTime)
{
	//Вычисляем время, прошедшее со старта анимации текущей стадии атаки на момент слеюдующего кадра
	AnimationSequenceStopTime = SwordAttack->FullDuration.Y - SwordAttack->Duration.Y + DeltaTime;
	//
	float AnimationSequenceDuration = SwordAttackAnimationSequence->GetTimeAtFrame(SwordAttackAnimationSequence->GetNumberOfFrames());
	//
	if(AnimationSequenceStopTime * SwordAttackAnimationSpeed > AnimationSequenceDuration)
	{
		AnimationSequenceStopTime = AnimationSequenceStopTime / SwordAttackAnimationSpeed;
	}
				
	//Вычислям позицию и поворот правой руки в нужный момент времени
	FTransform HandRTransform = FindBoneNewTransformRecurs(SwordAttackAnimationSequence, "hand_r", AnimationSequenceStopTime * SwordAttackAnimationSpeed);

	//Вычислям позицию и поворот центра меча в нужный момент времени
	FTransform SwordActorNextTransform = FTransform(HandRTransform.TransformRotation(SwordActorSocket->GetSocketLocalTransform().GetRotation()),
		HandRTransform.TransformPosition(SwordActorSocket->GetSocketLocalTransform().GetLocation()),
		FVector(1,1,1));

	//Массив объектов, которые пересечёт меч в следующем кадре
	TArray<TArray<FHitResult>> AllHits;
	for(int i = 0; i < SwordLength; i++)
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(SwordActor);
		QueryParams.AddIgnoredActor(BowActor);
		//Массив объектов, которые пересечёт одна из точек меча
		TArray<FHitResult> OutHits;
		GetWorld()->LineTraceMultiByChannel(OutHits,
			SwordActor->GetActorLocation() + SwordActor->GetActorUpVector() * -i,
			SwordActorNextTransform.GetLocation() + SwordActorNextTransform.GetRotation().GetUpVector() * -i,
			ECC_Camera,
			QueryParams);
					
		if(OutHits.Num() > 0)
		{
			AllHits.Add(OutHits);
		}
	}
				
	if(AllHits.Num() > 0)
	{
		//float MinDist = AllHits[0][0].Distance;
		float MinDist = 10000;
		//Ближайший объект, который заблокирует дальнейшее движение меча при коллизии
		FHitResult* BlockResult = nullptr;
		//Массив объектов, которые меч пересечёт не останавливаясь
		TArray<FHitResult> NonBlockResults;
		for(int i = 0; i < AllHits.Num(); i++)
		{
			for(int j = 0; j < AllHits[i].Num(); j++)
			{
				if(IsHitBlocking(AllHits[i][j]))
				{
					if(AllHits[i][j].Distance < MinDist)
					{
						MinDist = AllHits[i][j].Distance;
						BlockResult = &AllHits[i][j];
					}
				}
				else
				{
					NonBlockResults.Add(AllHits[i][j]);
				}
			}
		}
		if(!BlockResult && IsHitBlocking(AllHits[0][0]))
		{
			BlockResult = &AllHits[0][0]; 
		}
		//Удар заблокирован
		if(BlockResult)
		{
			//Расстояние от "центра" меча до точки, которая столкнётся с блокирующим объектам
			float SwordHitPointDistance = (BlockResult->TraceStart - SwordActor->GetActorLocation()).Length();
			AnimationSequenceStopTime -= DeltaTime;

			//Позиция и поворот руки (сокета) до столкновения меча с объектом
			FTransform SwordHitHandTransform = FindBoneNewTransformRecurs(SwordAttackAnimationSequence, "hand_r", AnimationSequenceStopTime * SwordAttackAnimationSpeed);
			//Позиция и поворот "центра" меча, до столкновения
			FTransform SwordHitTransform = FTransform(SwordHitHandTransform.TransformRotation(SwordActorSocket->GetSocketLocalTransform().GetRotation()),
				SwordHitHandTransform.TransformPosition(SwordActorSocket->GetSocketLocalTransform().GetLocation()),
				FVector(1,1,1));
			//Координаты точки меча, которая столкнётся с блокирующим объектом
			FVector SwordHitPointLocation = SwordHitTransform.GetLocation() + SwordHitTransform.GetRotation().GetUpVector() * -1 * SwordHitPointDistance;
			float MinDistanceToHitPoint = (SwordHitPointLocation - BlockResult->Location).Length();
			float MinK = 0;

			//Просматриваем каждый момент времени между кадрами, что бы найти момент анимации, при котором меч ближе всего к точке столкновения
			for (float k=DeltaTime/100; k < DeltaTime; k+= DeltaTime/20)
			{
				SwordHitHandTransform = FindBoneNewTransformRecurs(SwordAttackAnimationSequence, "hand_r", (AnimationSequenceStopTime + k) * SwordAttackAnimationSpeed);
				SwordHitTransform = FTransform(SwordHitHandTransform.TransformRotation(SwordActorSocket->GetSocketLocalTransform().GetRotation()),
					SwordHitHandTransform.TransformPosition(SwordActorSocket->GetSocketLocalTransform().GetLocation()),
					FVector(1,1,1));
				SwordHitPointLocation = SwordHitTransform.GetLocation() + SwordHitTransform.GetRotation().GetUpVector() * -1 * SwordHitPointDistance;
				float DistanceToHitPoint = (SwordHitPointLocation - BlockResult->Location).Length();
							
				if((SwordHitPointLocation - BlockResult->TraceStart).Length() < (BlockResult->Location - BlockResult->TraceStart).Length())
				{
					//DrawDebugSphere(GetWorld(), SwordHitPointLocation, 1, 10, FColor::Green, false, 150, 0);

					if(DistanceToHitPoint < MinDistanceToHitPoint)
					{
						MinDistanceToHitPoint = DistanceToHitPoint;
						MinK = k;
					}	
				}
				else
				{
					k = DeltaTime;
				}
			}
			AnimationSequenceStopTime += MinK;
			//SwordAbility->Duration.W = AnimationSequenceStopTime;
			//SwordAbilityAnimationSpeed = SwordAbility->AnimationSequence->GetTimeAtFrame(SwordAbility->AnimationSequence->GetNumberOfFrames()) / SwordAbility->FullDuration.Y;
			//
			AnimationSequenceStopTime *= SwordAttackAnimationSpeed;
			//
			SwordAttackPhase = ESwordAttackPhase::Rebound;

			//Отсеять те объекты NonBlockResults, что находятся дальше итоговой позиции
			//заблокированного меча
			for (int o = 0; o < NonBlockResults.Num(); o++)
			{
				if(NonBlockResults[o].Distance > BlockResult->Distance)
				{
					NonBlockResults.RemoveAt(o);
					o--;
				}
			}
		}
		//Удар не заблокирован 
		else
		{
			AnimationSequenceStopTime = 0;
						
			//Обработать NonBlockResults
			for (auto NonBlockResult : NonBlockResults)
			{
				//ConsoleLog(NonBlockResult.GetActor()->GetName());
			}
		}
	}
	//Удар не заблокирован 
	else
	{
		AnimationSequenceStopTime = 0;
	}
}

//Обработка удара по поставленному блоку
void AToG_Pawn_C::ProcessHit()
{
	if(SwordBlock)
	{
	//Позиция острия меча в момент удара по блоку
	FVector3d SwordEdgeLocation = SwordActor->GetActorLocation() + SwordActor->GetActorUpVector() * SwordLength * -1;
	//Трансформ руки, держащей меч в произвольный момент времени
	FTransform SwordHandBlockHitTransform;
	//Трансформ центра меча в произвольный момент времени
	FTransform SwordCenterBlockHitTransform;
	//Позиция острия меча в произвольный момент времени
	FVector3d SwordEdgeBlockHitLocation;
	//Дистанция между позицией острия меча в момент блока и предполагаемым началом анимации
	float SwordEdgePositionsDistance;
	float MinDistance = 100;
	float MinI = 0;
	switch (SwordBlockPhase)
	{
	case ESwordBlockPhase::Recovery:
		//Просматриваем каждый момент времени анимации Hit, что бы найти момент, при котором меч ближе всего к оригинальной позиции в момент удара по блоку
		for (float i = 0; i < SwordBlock->FullDuration.W; i += SwordBlock->FullDuration.W/100)
		{
			SwordHandBlockHitTransform = FindBoneNewTransformRecurs(AnimLibrary->SwordBlockHitAnimSequences, "hand_r", i);
			SwordCenterBlockHitTransform = FTransform(SwordHandBlockHitTransform.TransformRotation(SwordActorSocket->GetSocketLocalTransform().GetRotation()),
				SwordHandBlockHitTransform.TransformPosition(SwordActorSocket->GetSocketLocalTransform().GetLocation()),
				FVector(1,1,1));
			SwordEdgeBlockHitLocation = SwordCenterBlockHitTransform.GetLocation() + SwordCenterBlockHitTransform.GetRotation().GetUpVector() * SwordLength * -1;
			SwordEdgePositionsDistance = (SwordEdgeBlockHitLocation - SwordEdgeLocation).Length();

			if(SwordEdgePositionsDistance < MinDistance)
			{
				MinDistance = SwordEdgePositionsDistance;
				MinI = i;
			}
		}
		AnimationSequenceStopTime = MinI;
		SwordBlock->Duration.Z = SwordBlock->FullDuration.Z - MinI;
		SwordBlock->Duration.W = SwordBlock->FullDuration.W;
		//
		SwordBlockPhase = ESwordBlockPhase::Hit;
		break;
	case ESwordBlockPhase::Hold:
		//
		SwordBlockPhase = ESwordBlockPhase::Hit;
		AnimationSequenceStopTime = 0;
		break;
	}	
	}
}

//
void AToG_Pawn_C::ChangeSwordAttack(int NewAbilityIndex)
{
	SwordAttack = new FToG_SwordAttack(CheckForCombo(AvailableSwordAttacks[NewAbilityIndex]));
	SwordAttack->FullDuration *= SwordAttackAnimationSpeedCoefficient;
	SwordAttack->Duration *= SwordAttackAnimationSpeedCoefficient;
	SwordAttackId = SwordAttack->AnimationId;
	SwordAttackAnimationSequence = SwordAttack->AnimationSequences[FMath::RandRange(0,SwordAttack->AnimationSequences.Num()-1)];
}

//
void AToG_Pawn_C::ProcessSpell(float DeltaTime)
{
	if(PlayerSpell)
	{
		switch (PlayerSpellPhase)
		{
		case ESpellPhase::Zero:
			PlayerSpellPhase = PlayerSpell->StartPhase;
			SpellId = PlayerSpell->AnimationId;
			break;
		case ESpellPhase::Cast:
			if(PlayerSpell->Duration.X > 0)
			{
				PlayerSpell->Duration.X -= DeltaTime;
				PlayerSpell->SpellCast(ETimePhase::Update, this, DeltaTime);
			}
			else
			{
				PlayerSpellPhase = ESpellPhase::Hold;
				PlayerSpell->SpellCast(ETimePhase::Update, this, DeltaTime);
			}
			break;
		case  ESpellPhase::Hold:
			//Доработать
			//PlayerSpell->HoldUpdate(this, DeltaTime);
				if(PlayerSpell->Holdable)
				{
					PlayerSpell->SpellHold(ETimePhase::Update, this, DeltaTime);
				}
				else
				{
					if(PlayerSpell->Duration.Y > 0)
					{
						PlayerSpell->Duration.Y -= DeltaTime;
					}
					else
					{
						PlayerSpellPhase = ESpellPhase::Using;
						PlayerSpell->SpellUsing(ETimePhase::Update, this, DeltaTime);
					}	
				}
			break;
		case ESpellPhase::Using:
			if(PlayerSpell->Duration.Z == PlayerSpell->FullDuration.Z)
			{
				PlayerSpell->SpellUsing(ETimePhase::Start, this, DeltaTime);
			}
			if(PlayerSpell->Duration.Z > 0)
			{
				PlayerSpell->Duration.Z -= DeltaTime;
				PlayerSpell->SpellUsing(ETimePhase::Update, this, DeltaTime);
			}
			else
			{
				PlayerSpellPhase = ESpellPhase::Recovery;
				PlayerSpell->SpellUsing(ETimePhase::Final, this, DeltaTime);
			}
			break;
		case ESpellPhase::Recovery:
			if(PlayerSpell->Duration.W > 0)
			{
				PlayerSpell->Duration.W -= DeltaTime;
				PlayerSpell->SpellRecovery(ETimePhase::Update, this, DeltaTime);
			}
			else
			{
				PlayerSpell = nullptr;
				PlayerSpellPhase = ESpellPhase::Zero;
			}
			break;
		}
	}
}

//
void AToG_Pawn_C::DetermineSpell(int InputCase, int ActionButtonNumber)
{
	if(!PlayerSpell)
	{
		switch (PlayerSpellStance)
		{
		case EPLayerSpellStance::Fire:
			switch (ActionButtonNumber)
			{
		case 1:
			PlayerSpell = new FToG_FireBall(FVector4d(0.5f,0,0.15f,0.225f), true, 0, ESpellPhase::Cast);
				break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			PlayerSpell = new FToG_FireMeteor(FVector4d(1.4f,0,0.3f,0.5f), true, 1, ESpellPhase::Cast);
				break;
		case 5:
			break;
			}
		case EPLayerSpellStance::Force:
			switch (ActionButtonNumber)
			{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
			}
			break;
		case EPLayerSpellStance::Electricity:
			switch (ActionButtonNumber)
			{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
			}
			break;
		case EPLayerSpellStance::Source:
			switch (ActionButtonNumber)
			{
		case 1:
			PlayerSpell = new FToG_SourcePush(FVector4d(0,0,0.15f,0.4f), false, 0, ESpellPhase::Using);
				break;
		case 2:
			PlayerSpell = new FToG_SourceLift(FVector4d(0,0,0.15f,0.2f), false, 1, ESpellPhase::Using);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
			}
			break;
		case EPLayerSpellStance::Water:
			switch (ActionButtonNumber)
			{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
			}
			break;
		case EPLayerSpellStance::Ground:
			switch (ActionButtonNumber)
			{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
			}
			break;
		}
	}
	switch (InputCase)
	{
	case 0:
		break;
	case 1:
		if(PlayerSpellPhase == ESpellPhase::Hold)
		{
			if(PlayerSpell)
			{
				PlayerSpell->SpellUsing(ETimePhase::Start, this, GetWorld()->DeltaTimeSeconds);
				PlayerSpellPhase = ESpellPhase::Using;	
			}
		}
		else
		{
			if(PlayerSpell)
			{
				PlayerSpell->Holdable = false;
			}
		}
		break;
	}
}

//
void AToG_Pawn_C::DetermineSwordMovementSide()
{
	SwordMovementVector = FVector2d(InputMovement.X, InputMovement.Y);
	if(InputMovement.X > 0 && InputMovement.Y > 0)
	{
		SwordMovementSide = EMovementSide::ForwardRight;
	}
	if(InputMovement.X > 0 && InputMovement.Y < 0)
	{
		SwordMovementSide = EMovementSide::ForwardLeft;
	}
	if(InputMovement.X > 0 && InputMovement.Y == 0)
	{
		SwordMovementSide = EMovementSide::Forward;
	}
	if(InputMovement.X < 0 && InputMovement.Y > 0)
	{
		SwordMovementSide = EMovementSide::BackRight;
	}
	if(InputMovement.X < 0 && InputMovement.Y < 0)
	{
		SwordMovementSide = EMovementSide::BackLeft;
	}
	if(InputMovement.X < 0 && InputMovement.Y == 0)
	{
		SwordMovementSide = EMovementSide::Back;
	}
	if(InputMovement.X == 0 && InputMovement.Y > 0)
	{
		SwordMovementSide = EMovementSide::Right;
	}
	if(InputMovement.X == 0 && InputMovement.Y < 0)
	{
		SwordMovementSide = EMovementSide::Left;
	}
	if(InputMovement.X == 0 && InputMovement.Y == 0)
	{
		SwordMovementSide = EMovementSide::Zero;
	}
}

//
bool AToG_Pawn_C::WasMovementSideChanged(EMovementDirection OldDirection, EMovementDirection NewDirection, float DeltaTime)
{

	if(CurrentSwitchSideMovementTime > 0)
	{
		if(CurrentSwitchSideMovementTime - DeltaTime > 0)
		{
			CurrentSwitchSideMovementTime -= DeltaTime;	
		}
		else
		{
			CurrentSwitchSideMovementTime = 0;
		}
	}
	
	switch (OldDirection)
	{
	case EMovementDirection::Forward:
		if(
			NewDirection == EMovementDirection::Back ||
			NewDirection == EMovementDirection::BackRight ||
			NewDirection == EMovementDirection::BackLeft
			)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::ForwardRight || NewDirection == EMovementDirection::ForwardLeft)
			{
				if(CurrentSwitchSideMovementTime > 0)
				{
					CurrentSwitchSideMovementTime = 0;
					if(NewDirection != LastSideMovementDirection)
					{
						return true;
					}
				}
			}
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;	
		}
	case EMovementDirection::Back:
		if(NewDirection == EMovementDirection::Forward ||
	NewDirection == EMovementDirection::ForwardRight ||
	NewDirection == EMovementDirection::ForwardLeft)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;	
		}
	case EMovementDirection::Right:
		if(
			NewDirection == EMovementDirection::Left ||
			NewDirection == EMovementDirection::BackLeft ||
			NewDirection == EMovementDirection::ForwardLeft
			)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;	
		}
	case EMovementDirection::Left:
		if(
			NewDirection == EMovementDirection::Right ||
			NewDirection == EMovementDirection::ForwardRight ||
			NewDirection == EMovementDirection::BackRight
			)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;	
		}
	case EMovementDirection::ForwardRight:
		if(
			NewDirection == EMovementDirection::Left ||
			NewDirection == EMovementDirection::BackLeft ||
			NewDirection == EMovementDirection::ForwardLeft
			)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::Forward)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
				LastSideMovementDirection = EMovementDirection::ForwardRight;
			}
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;
		}
	case EMovementDirection::ForwardLeft:
		if(
			NewDirection == EMovementDirection::Right ||
			NewDirection == EMovementDirection::ForwardRight ||
			NewDirection == EMovementDirection::BackRight
			)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::Forward)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
				LastSideMovementDirection = EMovementDirection::ForwardLeft;
			}
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;
		}
	case EMovementDirection::BackRight:
		if(NewDirection == EMovementDirection::Left ||
NewDirection == EMovementDirection::BackLeft ||
NewDirection == EMovementDirection::ForwardLeft)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;
		}
	case EMovementDirection::BackLeft:
		if(NewDirection == EMovementDirection::Right ||
	NewDirection == EMovementDirection::ForwardRight ||
	NewDirection == EMovementDirection::BackRight)
		{
			return true;
		}
		else
		{
			if(NewDirection == EMovementDirection::Zero)
			{
				CurrentSwitchSideMovementTime = SwitchSideMovementTime;
			}
			return false;
		}
	case EMovementDirection::Zero:
		if(NewDirection == EMovementDirection::Zero)
		{
			return false;
		}
		else
		{
			if(CurrentSwitchSideMovementTime > 0)
			{
				CurrentSwitchSideMovementTime = 0;
				return true;
			}
			return false;	
		}
	default:
		return false;
	}
}

//
void AToG_Pawn_C::ProcessSprint(int Type)
{
	switch (Type)
	{
	case 0:
		if(DashPhase == EDashPhase::Zero)
		{
			DashPhase = EDashPhase::Dash;
			DashDirection = MovementDirection;
			CurrentDashTime = DashTime;
			StillSprint = true;
		}
		break;
	case 1:
		MovementType = EMovementType::Run;
		StillSprint = false;
		break;
	}
}

//
void AToG_Pawn_C::ArmsCameraRotation(float DeltaTime)
{
	PawnArmsMesh -> SetRelativeRotation(PawnArmsMesh->GetRelativeRotation() + CameraRotation);
}

//
void AToG_Pawn_C::ProcessDash(float DeltaTime)
{
	switch (DashPhase)
	{
	case EDashPhase::Zero:
		break;
	case EDashPhase::Dash:
		if(CurrentDashTime > 0)
		{
			CurrentDashTime -= DeltaTime;
			switch (DashDirection)
			{
			case EMovementDirection::Back:
				Movement = (GetActorForwardVector() * -1 + GetActorRightVector() * 0).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			case EMovementDirection::Zero:
				break;
			case EMovementDirection::Forward:
				Movement = (GetActorForwardVector() * 1 + GetActorRightVector() * 0).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			case EMovementDirection::Right:
				Movement = (GetActorForwardVector() * 0 + GetActorRightVector() * 1).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			case EMovementDirection::Left:
				Movement = (GetActorForwardVector() * 0 + GetActorRightVector() * -1).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			case EMovementDirection::ForwardRight:
				Movement = (GetActorForwardVector() * 1 + GetActorRightVector() * 1).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			case EMovementDirection::ForwardLeft:
				Movement = (GetActorForwardVector() * 1 + GetActorRightVector() * -1).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			case EMovementDirection::BackRight:
				Movement = (GetActorForwardVector() * -1 + GetActorRightVector() * 1).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			case EMovementDirection::BackLeft:
				Movement = (GetActorForwardVector() * -1 + GetActorRightVector() * -1).GetUnsafeNormal() * DashSpeed * DeltaTime;
				break;
			}
		}
		else
		{
			if(MovementDirection == EMovementDirection::Forward && StillSprint)
			{
				MovementType = EMovementType::Sprint;
			}
			DashPhase = EDashPhase::Zero;
			DashDirection = EMovementDirection::Zero;
			CurrentDashTime = DashTime;
		}
		break;
	case EDashPhase::Crash:
		break;
	}
}



//Temp function
bool AToG_Pawn_C::IsHitBlocking(FHitResult Hit)
{
	bool IsBlocked = false;
	if(Hit.GetActor()->Tags.Num() > 0 && Hit.GetActor()->Tags.Contains("Enemy"))
	{
		IsBlocked = Cast<AToG_EnemyPawn_C>(Hit.GetActor())->HitBySword(SwordAttack, this);
	}
	return IsBlocked;
}

//Temp rays for spell drawing in editor
void AToG_Pawn_C::DrawRaySpell()
{
	for (int i = 1; i < DrawingPoints.Num(); i++)
	{
		DrawDebugLine(
		GetWorld(),
	PawnCamera -> GetComponentLocation() + PawnCamera -> GetForwardVector() * 40
	+ PawnCamera -> GetRightVector() * DrawingPoints[i].Y +
	PawnCamera -> GetUpVector() * DrawingPoints[i].X,
	PawnCamera -> GetComponentLocation() + PawnCamera -> GetForwardVector() * 40
	+ PawnCamera -> GetRightVector() * DrawingPoints[i-1].Y +
	PawnCamera -> GetUpVector() * DrawingPoints[i-1].X,
	FColor::Yellow,
	false,
	0.0f,
	0.0f,
	0.5f
	);
	}
}

//Need to be replaced
void AToG_Pawn_C::CollectedSwordAttacks()
{
	AvailableSwordAttacks.Add(FToG_SwordAttack("Light left attack", FVector4d(0.3,0.25,0.5, 0.25), 10, ESwordAttackType::Parriable, ESwordAttackDirection::Left, 4, AnimLibrary->SwordLeftAttackAnimSequences));
	AvailableSwordAttacks.Add(FToG_SwordAttack("Light right attack", FVector4d(0.3,0.25,0.5, 0.25), 10, ESwordAttackType::Parriable, ESwordAttackDirection::Right, 3, AnimLibrary->SwordRightAttackAnimSequences));
	AvailableSwordAttacks.Add(FToG_SwordAttack("Light up attack", FVector4d(0.3,0.25,0.5, 0.25), 10, ESwordAttackType::Parriable, ESwordAttackDirection::Up, 1, AnimLibrary->SwordUpAttackAnimSequences));
	AvailableSwordAttacks.Add(FToG_SwordAttack("Light right from left attack", FVector4d(0.1,0.2,0.5, 0.2), 10, ESwordAttackType::Parriable, ESwordAttackDirection::Right, 5,  AnimLibrary->SwordUpAttackAnimSequences));
	AvailableSwordAttacks.Add(FToG_SwordAttack("Light left from right attack", FVector4d(0,0.4,0.5, 0.4), 10, ESwordAttackType::Parriable, ESwordAttackDirection::Left, 6,  AnimLibrary->SwordUpAttackAnimSequences));
}

//Need to be replaced
void AToG_Pawn_C::CollectedSwordCombos()
{
	// AvailableSwordCombos.Add(FToG_SwordCombo(
	// 	{
	// 		"Light left attack",
	// 		"Light right attack",
	// 		"Light left attack"
	// 	},
	// 	FToG_SwordAbility("Left-Right-Left combo", FVector3d(0,1.25,0.5), 30,
	// 		ESwordAttackType::UnParriable, ESwordAttackDirection::Left, 11)));
	// AvailableSwordCombos.Add(FToG_SwordCombo(
	// 	{
	// 		"Light right attack",
	// 		"Light left attack",
	// 		"Light up attack"
	// 	},
	// 	FToG_SwordAbility("Right-Left-Up combo",FVector3d(0,1.25,0.5), 40,
	// 		ESwordAttackType::UnParriable, ESwordAttackDirection::Up, 12)));
}

//Need to be replaced
void AToG_Pawn_C::CollectedSpells()
{
	
	TArray<FVector2d> Lightning = TArray<FVector2d>();
	Lightning.Add(FVector2d(0,0));
	Lightning.Add(FVector2d(-1,-1));
	Lightning.Add(FVector2d(-2,-2));
	Lightning.Add(FVector2d(-3,-3));
	Lightning.Add(FVector2d(-4,-4));
	Lightning.Add(FVector2d(-5,-5));
	Lightning.Add(FVector2d(-5,-4));
	Lightning.Add(FVector2d(-5,-3));
	Lightning.Add(FVector2d(-5,-2));
	Lightning.Add(FVector2d(-5,-1));
	Lightning.Add(FVector2d(-5,0));
	Lightning.Add(FVector2d(-6,-1));
	Lightning.Add(FVector2d(-7,-2));
	Lightning.Add(FVector2d(-8,-3));
	Lightning.Add(FVector2d(-9,-4));
	Lightning.Add(FVector2d(-10,-5));

	TArray<FVector2d> Delta = TArray<FVector2d>();
	Delta.Add(FVector2d(0,0));
	Delta.Add(FVector2d(-1,0));
	Delta.Add(FVector2d(-2,0));
	Delta.Add(FVector2d(-3,0));
	Delta.Add(FVector2d(-4,0));
	Delta.Add(FVector2d(-5,0));
	Delta.Add(FVector2d(-6,0));
	Delta.Add(FVector2d(-7,0));
	Delta.Add(FVector2d(-8,0));
	Delta.Add(FVector2d(-9,0));
	Delta.Add(FVector2d(-10,0));
	Delta.Add(FVector2d(-10,-1));
	Delta.Add(FVector2d(-10,-2));
	Delta.Add(FVector2d(-10,-3));
	Delta.Add(FVector2d(-10,-4));
	Delta.Add(FVector2d(-10,-5));
	Delta.Add(FVector2d(-9,-5));
	Delta.Add(FVector2d(-8,-5));
	Delta.Add(FVector2d(-7,-5));
	Delta.Add(FVector2d(-6,-5));
	Delta.Add(FVector2d(-5,-5));
	Delta.Add(FVector2d(-5,-4));
	Delta.Add(FVector2d(-5,-3));
	Delta.Add(FVector2d(-5,-2));
	Delta.Add(FVector2d(-5,-1));
	Delta.Add(FVector2d(-5,0));
	Delta.Add(FVector2d(-5,1));
	Delta.Add(FVector2d(-5,2));
	Delta.Add(FVector2d(-5,3));
	Delta.Add(FVector2d(-5,4));
	Delta.Add(FVector2d(-5,5));

	TArray<FVector2d> Alta = TArray<FVector2d>();
	Alta.Add(FVector2d(0,0));
	Alta.Add(FVector2d(0,1));
	Alta.Add(FVector2d(0,2));
	Alta.Add(FVector2d(0,3));
	Alta.Add(FVector2d(0,4));
	Alta.Add(FVector2d(0,5));
	Alta.Add(FVector2d(0,6));
	Alta.Add(FVector2d(0,7));
	Alta.Add(FVector2d(0,8));
	Alta.Add(FVector2d(0,9));
	Alta.Add(FVector2d(0,10));
	Alta.Add(FVector2d(1,10));
	Alta.Add(FVector2d(2,10));
	Alta.Add(FVector2d(3,10));
	Alta.Add(FVector2d(4,10));
	Alta.Add(FVector2d(5,10));
	Alta.Add(FVector2d(5,9));
	Alta.Add(FVector2d(5,8));
	Alta.Add(FVector2d(5,7));
	Alta.Add(FVector2d(5,6));
	Alta.Add(FVector2d(5,5));
	Alta.Add(FVector2d(4,5));
	Alta.Add(FVector2d(3,5));
	Alta.Add(FVector2d(2,5));
	Alta.Add(FVector2d(1,5));
	Alta.Add(FVector2d(0,5));
	Alta.Add(FVector2d(-1,5));
	Alta.Add(FVector2d(-2,5));
	Alta.Add(FVector2d(-3,5));
	Alta.Add(FVector2d(-4,5));
	Alta.Add(FVector2d(-5,5));

	TArray<FVector2d> Dalta = TArray<FVector2d>();
	Dalta.Add(FVector2d(0,0));
	Dalta.Add(FVector2d(-1,0));
	Dalta.Add(FVector2d(-2,0));
	Dalta.Add(FVector2d(-3,0));
	Dalta.Add(FVector2d(-4,0));
	Dalta.Add(FVector2d(-5,0));
	Dalta.Add(FVector2d(-6,0));
	Dalta.Add(FVector2d(-7,0));
	Dalta.Add(FVector2d(-8,0));
	Dalta.Add(FVector2d(-9,0));
	Dalta.Add(FVector2d(-10,0));
	Dalta.Add(FVector2d(-10,-1));
	Dalta.Add(FVector2d(-10,-2));
	Dalta.Add(FVector2d(-10,-3));
	Dalta.Add(FVector2d(-10,-4));
	Dalta.Add(FVector2d(-10,-5));
	Dalta.Add(FVector2d(-9,-5));
	Dalta.Add(FVector2d(-8,-5));
	Dalta.Add(FVector2d(-7,-5));
	Dalta.Add(FVector2d(-6,-5));
	Dalta.Add(FVector2d(-5,-5));
	Dalta.Add(FVector2d(-5,-4));
	Dalta.Add(FVector2d(-5,-3));
	Dalta.Add(FVector2d(-5,-2));
	Dalta.Add(FVector2d(-5,-1));
	Dalta.Add(FVector2d(-5,0));
	Dalta.Add(FVector2d(-5,1));
	Dalta.Add(FVector2d(-5,2));
	Dalta.Add(FVector2d(-5,3));
	Dalta.Add(FVector2d(-5,4));
	Dalta.Add(FVector2d(-5,5));
	Dalta.Add(FVector2d(-5,6));
	Dalta.Add(FVector2d(-5,7));
	Dalta.Add(FVector2d(-5,8));
	Dalta.Add(FVector2d(-5,9));
	Dalta.Add(FVector2d(-5,10));
	Dalta.Add(FVector2d(-4,10));
	Dalta.Add(FVector2d(-3,10));
	Dalta.Add(FVector2d(-2,10));
	Dalta.Add(FVector2d(-1,10));
	Dalta.Add(FVector2d(0,10));
	Dalta.Add(FVector2d(0,9));
	Dalta.Add(FVector2d(0,8));
	Dalta.Add(FVector2d(0,7));
	Dalta.Add(FVector2d(0,6));
	Dalta.Add(FVector2d(0,5));
	Dalta.Add(FVector2d(-1,5));
	Dalta.Add(FVector2d(-2,5));
	Dalta.Add(FVector2d(-3,5));
	Dalta.Add(FVector2d(-4,5));
	Dalta.Add(FVector2d(-5,5));
	Dalta.Add(FVector2d(-6,5));
	Dalta.Add(FVector2d(-7,5));
	Dalta.Add(FVector2d(-8,5));
	Dalta.Add(FVector2d(-9,5));
	Dalta.Add(FVector2d(-10,5));


	AvailableSpells.Add(FToG_SpellDrawing("Lightning",Lightning, 6));
	AvailableSpells.Add(FToG_SpellDrawing("Delta",Delta, 11));
	AvailableSpells.Add(FToG_SpellDrawing("Alta",Alta, 11));
	AvailableSpells.Add(FToG_SpellDrawing("Dalta",Dalta, 15));
}

//Need to be replaced
void AToG_Pawn_C::CollectedMotionSequences()
{
	TArray<FVector3d> a;
	TArray<FVector3d> b;
	//
	a.Add(FVector3d(0,0,0));
	a.Add(FVector3d(10,0,0));
	a.Add(FVector3d(45,0,0));
	a.Add(FVector3d(90,0,0));
	a.Add(FVector3d(150,0,0));
	//
	b.Add(FVector3d(0,0,0));
	b.Add(FVector3d(0,0,0));
	// b.Add(FVector3d(0,0,10));
	// b.Add(FVector3d(0,0,25));
	// b.Add(FVector3d(0,0,45));
	//
	AvailableMotionSequences.Add(FToG_MotionSequence("Left light attack motion", 1, a, b));
	AvailableMotionSequences.Add(FToG_MotionSequence("Right light attack motion", 1, a, b));
	AvailableMotionSequences.Add(FToG_MotionSequence("Up light attack motion", 1, a, b));
	AvailableMotionSequences.Add(FToG_MotionSequence("Down light attack motion", 1, a, b));

	AvailableMotionSequences.Add(FToG_MotionSequence("Left heavy attack motion", 1.5, a, b));
	AvailableMotionSequences.Add(FToG_MotionSequence("Right heavy attack motion", 1.5, a, b));
	AvailableMotionSequences.Add(FToG_MotionSequence("Up heavy attack motion", 1.5, a, b));
	AvailableMotionSequences.Add(FToG_MotionSequence("Down heavy attack motion", 1.5, a, b));

	AvailableMotionSequences.Add(FToG_MotionSequence("Left-Right-Left combo motion", 2.5, a, b));
	AvailableMotionSequences.Add(FToG_MotionSequence("Right-Left-Up combo motion", 2.5, a, b));

	AvailableMotionSequences.Add(FToG_MotionSequence("Parry motion", 1, a, b));
}

//
void AToG_Pawn_C::ShowCurrentCombatText()
{
	switch (PlayerCombatType)
	{
	case EPlayerCombatType::Zero:
		Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_0"))->SetText(FText().FromString(""));
		Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString(""));
		Cast<UImage>(PlayerInterface->GetWidgetFromName("Image_0"))->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EPlayerCombatType::Sword:
		Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_0"))->SetText(FText().FromString("Sword"));
		Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString(""));
		Cast<UImage>(PlayerInterface->GetWidgetFromName("Image_0"))->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EPlayerCombatType::Bow:
		Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_0"))->SetText(FText().FromString("Bow"));
		Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString(""));
		Cast<UImage>(PlayerInterface->GetWidgetFromName("Image_0"))->SetVisibility(ESlateVisibility::Visible);
		break;
	case EPlayerCombatType::Spell:
		Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_0"))->SetText(FText().FromString("Spell"));
		Cast<UImage>(PlayerInterface->GetWidgetFromName("Image_0"))->SetVisibility(ESlateVisibility::Visible);
		switch (PlayerSpellStance)
		{
		case EPLayerSpellStance::Electricity:
			Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString("Electricity"));
			break;
		case EPLayerSpellStance::Fire:
			Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString("Fire"));
			break;
		case EPLayerSpellStance::Ground:
			Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString("Ground"));
			break;
		case EPLayerSpellStance::Source:
			Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString("Source"));
			break;
		case EPLayerSpellStance::Water:
			Cast<UTextBlock>(PlayerInterface->GetWidgetFromName("TextBlock_1"))->SetText(FText().FromString("Water"));
			break;
		}
		break;
	}
}