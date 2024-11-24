// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MazeRunnerGameMode.generated.h"

UCLASS(minimalapi)
class AMazeRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMazeRunnerGameMode();

	void SpawnActorAtLocation();
};



