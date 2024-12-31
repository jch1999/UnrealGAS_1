// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealGAS_1GameMode.h"
#include "UnrealGAS_1Character.h"
#include "UObject/ConstructorHelpers.h"

AUnrealGAS_1GameMode::AUnrealGAS_1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
