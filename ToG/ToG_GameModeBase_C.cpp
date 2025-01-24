// Fill out your copyright notice in the Description page of Project Settings.


#include "ToG_GameModeBase_C.h"

AToG_GameModeBase_C::AToG_GameModeBase_C()
{
	PlayerControllerClass = AToG_PlayerController_C::StaticClass();
	DefaultPawnClass = AToG_Pawn_C::StaticClass();
}
