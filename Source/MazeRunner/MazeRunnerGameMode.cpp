// Copyright Epic Games, Inc. All Rights Reserved.

#include "MazeRunnerGameMode.h"
#include "MazeRunnerPlayerController.h"
#include "MazeRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMazeRunnerGameMode::AMazeRunnerGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMazeRunnerPlayerController::StaticClass();

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