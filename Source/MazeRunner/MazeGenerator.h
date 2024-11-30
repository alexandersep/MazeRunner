// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS(Blueprintable)
class MAZERUNNER_API AMazeGenerator : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMazeGenerator();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> wallsGrid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> cornersGrid;
	FVector mazeStart;
	FVector mazeEnd;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> solution;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	TArray<int32> generateMaze(int rows, int columns);
};
