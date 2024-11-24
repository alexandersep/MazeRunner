// Copyright Epic Games, Inc. All Rights Reserved.

#include "MazeRunnerGameMode.h"
#include "MazeRunnerPlayerController.h"
#include "MazeRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Wall.h"

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

    SpawnActorAtLocation();
}

void AMazeRunnerGameMode::SpawnActorAtLocation()
{
    // Define the spawn location and rotation
    FVector SpawnLocation(0.0f, 0.0f, 0.0f);  // x, y, z coordinates
    FRotator SpawnRotation(0.0f, 0.0f, 0.0f);  // Default rotation (no rotation)

    // Specify the class of the actor you want to spawn
    TSubclassOf<AActor> ActorToSpawn = AWall::StaticClass();  // Replace with your actor class

    UE_LOG(LogTemp, Warning, TEXT("Testing if SpawnACtorAtLocation Called!"));


    // Check if the actor class is valid
    if (ActorToSpawn)
    {
        // Get the current world
        UWorld* World = GetWorld();
        if (World)
        {
            // Spawn the actor at the specified location and rotation
            AActor* SpawnedActor = World->SpawnActor<AWall>(ActorToSpawn, SpawnLocation, SpawnRotation);

            if (SpawnedActor)
            {
                UE_LOG(LogTemp, Warning, TEXT("Actor spawned successfully!"));
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Actor not spawned successfully!"));
            }
        }
    }
}