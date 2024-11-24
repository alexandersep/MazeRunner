// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>("WallMesh");
	// attach to root component
	RootComponent = WallMesh;
	// Optionally, set the mesh for the wall
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshAsset(TEXT("StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"));
	if (WallMeshAsset.Succeeded())
	{
		WallMesh->SetStaticMesh(WallMeshAsset.Object);
		UE_LOG(LogTemp, Warning, TEXT("Wall mesh should have succeeded!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Wall mesh not succeeded!"));
	}
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

