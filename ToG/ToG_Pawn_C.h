// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "ToG_CommonAbstractions.h"
#include "ToG_EnemyPawn_C.h"
#include "FToG_SwordAttack.h"
#include "FToG_MovementAbility.h"
#include "FToG_SwordCombo.h"
#include "FToG_SwordBlock.h"
#include "FToG_SpellDrawing.h"
#include "FToG_MotionSequence.h"
#include "FToG_Spell.h"
#include "TechnicalFunctions.h"
#include "ToG_PlayerAnimationLibrary.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Engine/Canvas.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToG_Pawn_C.generated.h"

using namespace TechnicalFunctionsNamespace;

UENUM()
enum class EPlayerCombatType : uint8
{
	Zero,
	Sword,
	Bow,
	Spell
};

UENUM(BlueprintType)
enum class EPlayerCombatModeChoosing : uint8
{
	Zero,
	Show,
	Hold,
	Hide
};

UENUM()
enum class EPlayerDrawMode : uint8
{
	On,
	Off
};

UENUM(BlueprintType)
enum class EPLayerSpellStance : uint8
{
	Source,
	Fire,
	Water,
	Ground,
	Electricity,
	Force
};

USTRUCT()
struct FPlayerInputPermission
{
	GENERATED_BODY()
public:
	bool X;
	bool Y;
	bool Z;
	
};

UCLASS()
class TOG_API AToG_Pawn_C : public APawn
{
	GENERATED_BODY()

public:
	//Camera for this pawn
	UPROPERTY(EditAnywhere, Category = "Components")
	UCameraComponent* PawnCamera;
	//Mesh for this pawn
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* PawnMesh;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* PawnArmsMesh;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* PawnSkeletalMeshFull;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* PawnSkeletalMeshLegs;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* PawnSkeletalMeshArms;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UToG_PlayerAnimationLibrary* AnimLibrary;
	//
	UPROPERTY(EditAnywhere, Category = "Components")
	UUserWidget* PlayerInterface;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Settings")
	EPawnControlType PawnControlType;

	//Permission of pawn's movement's input component 
	UPROPERTY(EditAnywhere, Category = "Input Settings")
	FPlayerInputPermission MovementInputPermissions;
	//Permission of pawn's rotation's input component 
	UPROPERTY(EditAnywhere, Category = "Input Settings")
	FPlayerInputPermission RotationInputPermissions;
	
	//Vector of next movement for the pawn
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector Movement;
	//Vector of next movement's input component 
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector InputMovement;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementDirection MovementDirection;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementDirection NewMovementDirection;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementDirection LastSideMovementDirection;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SwitchSideMovementTime;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CurrentSwitchSideMovementTime;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementType MovementType;
	//Speed of current pawn's movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CurrentSpeed;
	//
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TargetSpeed;
	//Speed of walking
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed;
	//Speed of sprinting
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed;
	//Speed of running forward
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunForwardSpeed;
	//Speed of running back
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunBackSpeed;
	//Speed of running to side
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSideSpeed;
	//Speed of running to side and forward
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSideForwardSpeed;
	//Speed for sword combat
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SwordCombatSpeed;
	//Speed for sword attack
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SwordCombatAttackSlowSpeed;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SpeedAcceleration;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StartSpeedCoefficient;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EDashPhase DashPhase;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementDirection DashDirection;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashSpeed;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashTime;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CurrentDashTime;
	//
	bool StillSprint;
	
	//Vector of next rotation for the pawn
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator Rotation;
	//Vector of next rotation for the camera
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator CameraRotation;
	//Vector of next pawn rotation's input component 
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator InputPawnRotation;
	//Vector of next camera rotation's input component 
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator InputCameraRotation;
	//Speed of pawn's rotation
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float TurnSpeed;

	//Camera/Head lookup speed
	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float LookUpSpeed;
	//Camera/Head lookup max positive angle
	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float LookUpAngleMax;
	//Camera/Head lookup max negative angle
	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float LookUpAngleMin;
	
	//Player's combat type status
	UPROPERTY(EditAnywhere, Category = "Combat Settings")
	EPlayerCombatType PlayerCombatType;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	bool ShowCombatModeList;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Settings")
	EPlayerCombatModeChoosing PlayerCombatModeChoosing;
	//
	UPROPERTY(EditAnywhere, Category = "Combat Settings")
	FVector3d TimeToChooseMode;
	//
	float CurrentTimeToChooseMode;
	//Current active sword attack
	FToG_SwordAttack* SwordAttack;
	//Current active sword block
	FToG_SwordBlock* SwordBlock;
	//
	FToG_SwordAttack* NextSwordAttack;
	//
	FVector2d NextSwordAttackAllowedTime;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	EMovementSide SwordMovementSide;
	//
	FVector2d SwordMovementVector;
	//Temp sword actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	AActor* SwordActor;
	//Temp sword actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow Combat Settings")
	AActor* BowActor;
	//
	UPROPERTY(EditAnywhere, Category = "Sword Combat Settings")
	const USkeletalMeshSocket* SwordActorSocket;
	//
	UPROPERTY(EditAnywhere, Category = "Bow Combat Settings")
	const USkeletalMeshSocket* BowActorSocket;
	//Direction of player's sword attack
	ESwordAttackDirection PlayerSwordAttackDirection;
	//Available sword attacks that player can use
	TArray<FToG_SwordAttack> AvailableSwordAttacks;
	//Available sword attack combos that player can use
	TArray<FToG_SwordCombo> AvailableSwordCombos;
	//Several last sword abilities, that can be combine to combo
	TArray<FToG_SwordAttack> LastSwordAttacks;
	//
	float SwordAttackLifeTime;
	//
	float LastSwordAttackTime;

	//Player's draw spell's mode status
	UPROPERTY(EditAnywhere, Category = "Spell Settings")
	EPlayerDrawMode PlayerDrawMode;
	//
	TArray<FVector2d> DrawingPoints;
	//
	FVector2d NextPointOffset;
	//
	double NextPointOffsetSize;
	//
	TArray<FToG_SpellDrawing> AvailableSpells;
	//
	int MinDrawingPoints;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings")
	float AnimationStartPosition;
	//
	TArray<FToG_MotionSequence> AvailableMotionSequences;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	ESwordAttackPhase SwordAttackPhase;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	ESwordBlockPhase SwordBlockPhase;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	bool IsBlockHold;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	int SwordAttackId;
	//
	float SwordLength;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	float AnimationSequenceStopTime;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings")
	UAnimSequence* SwordAttackAnimationSequence;
	//Скорость воспроизведения анимационного ролика, соответствующая времени атаки
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	float SwordAttackAnimationSpeed;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	float SwordAttackAnimationSpeedCoefficient;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Combat Settings")
	float SwordNextAttackAllowedTime;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Settings")
	EPLayerSpellStance PlayerSpellStance;
	//
	FToG_Spell* PlayerSpell;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Settings")
	int SpellId;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Settings")
	ESpellPhase PlayerSpellPhase;
	//
	UPROPERTY(EditAnywhere, Category = "Spell Settings")
	TArray<const USkeletalMeshSocket*> SpellModeSockets;
	//
	UPROPERTY(EditAnywhere, Category = "Spell Settings")
	TArray<AActor*> SpellModeActors;
	
	// Sets default values for this pawn's properties
	AToG_Pawn_C();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Determine player's input pawn's movement and rotation component 
	void DetermineCurrentInput(float DeltaTime);

	//Change player's input with current permissions
	void CorrectInputByPermissions();

	// Called to move player's pawn forward or back
	void MoveCharacterForwardBack(float AxisValue);

	// Called to move player's pawn right or left
	void MoveCharacterRightLeft(float AxisValue);

	// Called to rotate player's pawn
	void RotateCharacter(float AxisValue);

	// Called to rotate player's pawn's camera
	void LookUpCharacter(float AxisValue);

	// Need to be replaced
	void RotateCamera(float AxisValue);

	//Called to process combat type
	void ProcessCombatType();

	//
	void CombatModeActionUpdate(float DeltaTime);

	//Called to process draw spell mode
	void ProcessDrawMode();

	//
	void DrawSpell(float DeltaTime);

	//
	void DrawRayShape(TArray<FVector2d> Shape);

	//
	double CalculateShapesDifference(TArray<FVector2d> Spell, TArray<FVector2d> Drawing);

	//
	FToG_SpellDrawing CorrelateDrawingToSpell(TArray<FToG_SpellDrawing> Spells);

	// Process light attack start depending on combat type
	void ProcessActionOne(int Type);

	// Process heavy attack start depending on combat type
	void ProcessActionTwo(int Type);

	// Process heavy attack start depending on combat type
	void ProcessActionTwo_OldBlock(int Type);

	// 
	void ProcessActionThree(int Type);

	// 
	void ProcessActionFour(int Type);

	// 
	void ProcessActionFive(int Type);

	//
	FToG_SwordAttack CheckForCombo(FToG_SwordAttack NewSwordAttack);

	//
	void DetermineNewPawnLocation();

	//
	void DetermineNewPawnRotation();

	//
	void DetermineNewCameraRotation();

	//
	void NullifyMovementAndRotation();

	//
	void PlayerStateMachine(float DeltaTime);

	//
	void ProcessSwordAbilities(float DeltaTime);

	//
	void LastSwordAbilitiesClean(float DeltaTime);

	//
	void MoveSwordActorToSocket(float DeltaTime);

	//
	FTransform FindBoneNewTransform(UAnimSequence* PawnAnimation, FName BoneName, FTransform BoneRoot, double NextFrameTime);

	//
	FTransform FindBoneNewTransformRecurs(UAnimSequence* PawnAnimation, FName BoneName, double NextFrameTime);

	//
	void ProcessSwordHits(float DeltaTime);

	//
	void ProcessHit();

	//
	void ChangeSwordAttack(int NewAttackIndex);

	//
	void ProcessSpell(float DeltaTime);

	//
	void DetermineSpell(int InputCase, int ActionButtonNumber);

	//
	void ProcessCombatModeAction(int Type);

	//
	void ProcessMouseSelect(float AxisValue);
	
	//
	void DetermineSwordMovementSide();

	//
	bool WasMovementSideChanged(EMovementDirection OldDirection, EMovementDirection NewDirection, float DeltaTime);

	//
	void ProcessSprint(int Type);

	//
	void ArmsCameraRotation(float DeltaTime);

	//
	void ProcessDash(float DeltaTime);
	
	
	//Temp function
	bool IsHitBlocking(FHitResult Hit);
	
	//Temp rays for spell drawing in editor
	void DrawRaySpell();
	
	//Need to be replaced
	void CollectedSwordAttacks();

	//Need to be replaced
	void CollectedSwordCombos();
	
	//Need to be replaced
	void CollectedSpells();
	
	//Need to be replaced
	void CollectedMotionSequences();

	//
	void ShowCurrentCombatText();

};