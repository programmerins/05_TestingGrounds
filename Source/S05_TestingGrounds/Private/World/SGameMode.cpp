// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SGameMode.h"
#include "SHUD.h"
#include "BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASGameMode::ASGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Player/Behavior/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASHUD::StaticClass();
}
