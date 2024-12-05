// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"
#include "MazeRunnerMazeGenerator.h"
#include "MazeRunnerUtils.h"
#include <iostream>
#include <vector>

DEFINE_LOG_CATEGORY_STATIC(SomeObjectSub, Log, All);

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<int32> AMazeGenerator::generateMaze(int rows, int columns)
{
	auto generator = MazeRunnerMazeGenerator::create(rows, columns);
	if (!generator) {
		return {};
	}
	MazeRunnerMazeGenerator maze = generator.value();
	maze.kruskals_algorithm();
	pair<pair<int,int>, pair<int,int>> pos = maze.getBeginEnd();
	vector<vector<int>> maze2d = maze.getMazeMap();
	vector<int> mazeMap = Utils::flatten(maze2d);
	
	// map vector<int> to TArray<int32>
	TArray<int32> gridArr;
	gridArr.SetNumUninitialized(mazeMap.size());
	for (int i = 0; i < mazeMap.size(); i++) {
		gridArr[i] = mazeMap[i];
	}
	this->wallsGrid = gridArr;
	
	// get and map corners
	vector<vector<int>> corners2d = maze.getCornerMap();
	vector<int> cornersMap = Utils::flatten(corners2d);
	TArray<int32> cornersArr;
	cornersArr.SetNumUninitialized(cornersMap.size());
	for (int i = 0; i < cornersMap.size(); i++) {
		cornersArr[i] = cornersMap[i];
	}

	this->cornersGrid = cornersArr;

	// get and map keys
	vector<vector<int>> tileContents2d = maze.getTileContentsMap(pos.first, pos.second);
	vector<int> tileContentsFlat = Utils::flatten(tileContents2d);
	TArray<int32> tileContents32;
	tileContents32.SetNumUninitialized(tileContentsFlat.size());
	for (int i = 0; i < tileContentsFlat.size(); i++) {
		tileContents32[i] = tileContentsFlat[i];

	this->tileContentsGrid = tileContents32;

	// map pos.first to FVector mazeStart
	this->mazeStart = FVector(static_cast<float>(pos.first.first), static_cast<float>(pos.first.second), 0.0f);

	// map pos.second to FVector mazeEnd
	this->mazeEnd = FVector(static_cast<float>(pos.second.first), static_cast<float>(pos.second.second), 0.0f);

	// map vector<Cell> to TArray<FVector>
	vector<pair<int,int>> solCells = maze.solve(pos.first, pos.second);
	TArray<FVector> fVecArr;
	fVecArr.Reserve(solCells.size()); // Reserve memory for efficiency

	for (const pair<int,int>& cell : solCells)
	{
		fVecArr.Add(FVector(static_cast<float>(cell.first), static_cast<float>(cell.second), 0.0f));
	}

	this->solution = fVecArr;

	return gridArr;
}
