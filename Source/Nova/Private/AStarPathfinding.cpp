// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarPathfinding.h"




// Sets default values
AAStarPathfinding::AAStarPathfinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAStarPathfinding::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AAStarPathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAStarPathfinding::setStartEndCoord(int MapXLen,
	int MapYLen, int BoxWidth, int boxZCoord,
	FVector BoxStartCorrd, FVector BoxEndCoord)
{
	this->xLen = MapXLen;
	this->yLen = MapYLen;
	this->width = BoxWidth;
	this->zCoord = boxZCoord;
	this->startBlockCorrd = FVector(BoxStartCorrd.X - width / 2, BoxStartCorrd.Y - width / 2, BoxStartCorrd.Z);
	this->endBlockCorrd =  FVector(BoxEndCoord.X - width / 2, BoxEndCoord.Y - width / 2, BoxEndCoord.Z);
	initBlockDistances();
	searchedCoords.Add(startBlockCorrd);
}
//
//
void AAStarPathfinding::initBlockDistances() {
	for (int y = 0; y < yLen; y += width) {
		TArray<float> tempGDistance;
		TArray<float> tempHDistance;
		TArray<FVector> tempDirection;
		for (int x = 0; x < xLen; x += width ) {
			tempGDistance.Add(1000000);
			tempHDistance.Add(abs(endBlockCorrd[0] - x) + abs(endBlockCorrd[1] - y));
			tempDirection.Add(FVector());
		}
		gDistances.Add(tempGDistance);
		hDistances.Add(tempHDistance);
		directions.Add(tempDirection);
	}
}


void AAStarPathfinding::updateQueueCoords(int gDistance) {
	for (FVector coord : searchedCoords.Array()) {
		for (int dX : {-width, width}) {
			for (int dY : {-width, width}) {
				FVector tempVector = coord;
				tempVector.X += dX;
				tempVector.Y += dY;
				if (
					tempVector.X >= xLen ||
					tempVector.X < 0 ||
					tempVector.Y >= yLen ||
					tempVector.Y < 0
					) {
					continue;
				}
				
				FHitResult BoxHit;
				if (UKismetSystemLibrary::BoxTraceSingle(
					this,
					coord,
					tempVector,
					FVector(width/2, width/2, width/2),
					this->GetActorRotation(),
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					TArray<AActor*>(),
					EDrawDebugTrace::None,
					BoxHit,
					true))
				{
					continue;
				}
				if (queueCoords.Contains(tempVector)) {
					gDistances[tempVector.Y][tempVector.X] = gDistance;

					directions[tempVector.Y][tempVector.X] = FVector(dX, dY, 0);
					queueCoords.Add(tempVector);
				}
			}
		}

	}
}


void AAStarPathfinding::solve() {
	FVector nextStepCoord = startBlockCorrd;
	int currentGDistance = width;
	while (true) {
		float leastFDistance = 1000000;
		float leastHDistance = 1000000;
		for (FVector coord : queueCoords.Array()) {
			float fDistance = gDistances[coord.Y][coord.X] + hDistances[coord.Y][coord.X];
			if (fDistance < leastFDistance) {
				leastFDistance = fDistance;
				nextStepCoord = coord;
			}
			else if (fDistance == leastFDistance) {
				float hDistance = hDistances[coord.Y][coord.X];
				if (hDistance < leastHDistance) {
					leastHDistance = hDistance;
					nextStepCoord = coord;
				}
			}
		}

		if (nextStepCoord == endBlockCorrd) {
			break;
		}

		updateQueueCoords(currentGDistance);

		currentGDistance += width;
	}
	backTrace();
}

void AAStarPathfinding::backTrace() {
	FVector currentCoord = endBlockCorrd;
	while (true) {
		if (currentCoord == startBlockCorrd) {
			break;
		}
		FVector coordDirection = directions[currentCoord.Y][currentCoord.X];
		solvedResultCoords.Add(coordDirection);
		currentCoord = FVector(currentCoord.X - coordDirection.X, currentCoord.Y - coordDirection.Y, 0);
	}
}

FVector AAStarPathfinding::getNextStepDirection() {
	FVector value = solvedResultCoords[-1];
	solvedResultCoords.Pop();
	return value;
}