// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AStarPathfinding.generated.h"

UCLASS()
class NOVA_API AAStarPathfinding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAStarPathfinding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// a vector of coords that is the path of the solustion
	TArray<FVector> solvedResultCoords;

	// step to start
	TArray<TArray<float>>  gDistances;
	// abs distance to end
	TArray<TArray<float>>  hDistances;

	// flow directions
	TArray<TArray<FVector>> directions;

	// store block corrd that is alread searced
	TSet<FVector> searchedCoords;

	// store block corrd that is ready for search
	TSet<FVector> queueCoords;
	
	int xLen, yLen, width, zCoord;
	FVector startBlockCorrd;
	FVector endBlockCorrd;


	void initBlockDistances();

	void updateQueueCoords(int gDistance, TArray<AActor*> IgnoreWhenTracing);

	UFUNCTION(BlueprintCallable)
	void setStartEndCoord(int MapXLen, int MapYLen, int BoxWidth, int boxZCoord, FVector BoxStartCorrd, FVector BoxEndCoord);
	
	UFUNCTION(BlueprintCallable)
	void solve(TArray<AActor*> IgnoreWhenTracing);

	UFUNCTION(BlueprintCallable)
	FVector getNextStepDirection();

	void backTrace();
	
	
};
