// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTest002GameMode.h"
#include "MTest002PlayerController.h"
#include "MTest002Character.h"
#include "UObject/ConstructorHelpers.h"

AMTest002GameMode::AMTest002GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMTest002PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}