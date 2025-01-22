// Copyright Epic Games, Inc. All Rights Reserved.

#include "Descent2025GameMode.h"
#include "Descent2025Character.h"
#include "UObject/ConstructorHelpers.h"

ADescent2025GameMode::ADescent2025GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
