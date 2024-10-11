// Copyright Epic Games, Inc. All Rights Reserved.

#include "MagiciansGameMode.h"
#include "MagiciansCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMagiciansGameMode::AMagiciansGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
