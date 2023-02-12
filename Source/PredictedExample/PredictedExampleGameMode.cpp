// Copyright Epic Games, Inc. All Rights Reserved.

#include "PredictedExampleGameMode.h"
#include "PredictedExampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

APredictedExampleGameMode::APredictedExampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
