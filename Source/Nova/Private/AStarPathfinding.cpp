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
	solvedResultCoords.Empty();
	gDistances.Empty();
	hDistances.Empty();
	directions.Empty();
	searchedCoords.Empty();
	queueCoords.Empty();

	
	this->xLen = MapXLen;
	this->yLen = MapYLen;
	this->width = BoxWidth;
	this->zCoord = boxZCoord;
	this->startBlockCorrd = FVector(BoxStartCorrd.X - width / 2, BoxStartCorrd.Y - width / 2, BoxStartCorrd.Z);
	this->endBlockCorrd =  FVector(BoxEndCoord.X - width / 2, BoxEndCoord.Y - width / 2, BoxEndCoord.Z);
	initBlockDistances();
	// searchedCoords.Add(startBlockCorrd);
	// GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Black, FString("setStartEndCoord Done!"));
	//
}
//
//
void AAStarPathfinding::initBlockDistances() {
	
	for (int y = 0; y < yLen; y += width) {
		TArray<float> tempGDistance;
		TArray<float> tempHDistance;
		TArray<FVector> tempDirection;
		for (int x = 0; x < xLen; x += width ) {
			tempGDistance.Add(0);
			tempHDistance.Add( sqrt(pow(abs(endBlockCorrd.X - x), 2) + pow(abs(endBlockCorrd.Y - y),2) * 2));
			tempDirection.Add(FVector());
		}
		gDistances.Add(tempGDistance);
		hDistances.Add(tempHDistance);
		directions.Add(tempDirection);
	}
	// GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Black, FString("initBlockDistances Done!"));
	// GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Black, FString::FromInt(directions.Num()));
}


void AAStarPathfinding::updateQueueCoords(int gDistance, TArray<AActor*> ignoreActors, TArray<AActor*> noIgnoreActors) {
	TArray<FVector> dirs = {
		FVector(width, 0, 0),FVector(-width, 0, 0),
		FVector(0, width, 0),FVector(0, -width, 0),
		FVector(width, width, 0),FVector(width, -width, 0),
		FVector(-width, width, 0),FVector(-width, -width, 0),
	};
	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Black, FString::FromInt(searchedCoords.Num()));
	
	for (FVector coord : searchedCoords) {
		for (FVector dir : dirs)
		{
			FVector tempVector = coord;
			tempVector.X += dir.X;
			tempVector.Y += dir.Y;
			if (
			tempVector.X >= xLen ||
			tempVector.X < 0 ||
			tempVector.Y >= yLen ||
			tempVector.Y < 0
			) {
				// UE_LOG(LogTemp, Log, TEXT("passed"))
				continue;
			}
			// GEngine->AddOnScreenDebugMessage(9, 60.f, FColor::Black, Start.ToString());
			// GEngine->AddOnScreenDebugMessage(10, 60.f, FColor::Black,  End.ToString());

			// FRONT 从前面的上面往中间扫描
			FHitResult BoxHit;
			if (UKismetSystemLibrary::BoxTraceSingle(
			this,
			FVector(tempVector.X + width / 2, tempVector.Y + width / 2, coord.Z + width + 10),																																																		
			FVector(tempVector.X + width / 2, tempVector.Y + width / 2, coord.Z + 10),
			FVector(1.f , 1.f, 1.f),
			this->GetActorRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ignoreActors,
			EDrawDebugTrace::None,
			BoxHit,
			true))
			{
				if (noIgnoreActors.Contains(BoxHit.GetActor()))
				{
					continue;
				}
				
				// UP 从前面的上上面往上面扫描
				if (UKismetSystemLibrary::BoxTraceSingle(
					this,
					FVector(tempVector.X + width / 2, tempVector.Y + width / 2, coord.Z + width + width + 10),
					FVector(tempVector.X + width / 2, tempVector.Y + width / 2, tempVector.Z + width + 10),
					FVector(1.f , 1.f, 1.f),
					this->GetActorRotation(),
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					ignoreActors,
					EDrawDebugTrace::None,
					BoxHit,
					true))
				{
					// 无法行动
					continue;
				}
				// 否则上行
				dir.Z = width;
				
			}
			// Down
			// scan 前面往下扫描一格
			 else if (UKismetSystemLibrary::BoxTraceSingle(
			this,
			FVector(tempVector.X + width / 2, tempVector.Y + width / 2, coord.Z + 10),
			FVector(tempVector.X + width / 2, tempVector.Y + width / 2, tempVector.Z - width + 10),
			FVector(1.f , 1.f, 1.f),
			this->GetActorRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ignoreActors,
			EDrawDebugTrace::None,
			BoxHit,
			true))
			{
				// 扫描到了物品
				if (noIgnoreActors.Contains(BoxHit.GetActor()))
				{
					// 如果是角色 直接推出
					continue;
				}
				// 否则直行
			} else if (UKismetSystemLibrary::BoxTraceSingle(
				this,
				FVector(tempVector.X + width / 2, tempVector.Y + width / 2, coord.Z- width + 10),
				FVector(tempVector.X + width / 2, tempVector.Y + width / 2, tempVector.Z - width - width + 10),
				FVector(1.f , 1.f, 1.f),
				this->GetActorRotation(),
				ETraceTypeQuery::TraceTypeQuery1,
				false,
				ignoreActors,
				EDrawDebugTrace::None,
				BoxHit,
				true))
			{
				// 如果没有扫描到，继续扫描一格
				// 扫描到了物品
				if (noIgnoreActors.Contains(BoxHit.GetActor()))
				{
					// 如果是角色 直接推出
					continue;
				}
				// 否下行
				dir.Z = -width;

			} else {
				// 依然没有陆地，无法通过
				continue;
			}
			tempVector.Z += dir.Z;
			if (!queueCoords.Contains(tempVector) && !searchedCoords.Contains(tempVector)) {
				
				gDistances[tempVector.Y / width][tempVector.X / width] = gDistance;
				directions[tempVector.Y / width][tempVector.X / width] = dir;

				queueCoords.Add(tempVector);
			}
			
		}
	}
	UE_LOG(LogTemp, Log, TEXT("updateQueueCoords Done!"))
	// GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Black, FString(""));
}


bool AAStarPathfinding::solve(TArray<AActor*> ignoreActors, TArray<AActor*> noIgnoreActors) {
	FVector nextStepCoord = startBlockCorrd;
	
	int currentGDistance = width;
	int count = 0;
	while (true) {
		float leastFDistance = 10000000;
		float leastHDistance = 10000000;

		searchedCoords.Add(nextStepCoord);
		queueCoords.Remove(nextStepCoord);
		updateQueueCoords(currentGDistance, ignoreActors, noIgnoreActors);

		
		for (FVector coord : queueCoords) {
			float fDistance = gDistances[coord.Y / width][coord.X / width] + hDistances[coord.Y / width][coord.X / width];
			if (fDistance < leastFDistance)
			{
				leastFDistance = fDistance;
				nextStepCoord = coord;
			} else if (fDistance == leastFDistance)
			{
			 	float hDistance = hDistances[coord.Y / width][coord.X / width];
			 	if (hDistance < leastHDistance) {
			 		leastHDistance = hDistance;
			 		nextStepCoord = coord;
			 	}
			}
		}
		
		if (abs(nextStepCoord.X - endBlockCorrd.X) <= 15 &&
		abs(nextStepCoord.Y - endBlockCorrd.Y) <= 15) {
			UE_LOG(LogTemp, Log, TEXT("found! break"))
			break;
		}

		currentGDistance += width;
		count += 1;
		if (count > ((xLen / width) * (yLen / width)))
		{
			return false;
		}
	}
	if (!backTrace())
	{
		return false;
	}
	return true;
	// GEngine->AddOnScreenDebugMessage(5, 60.f, FColor::Black, FString("solve Done!"));
}

bool AAStarPathfinding::backTrace() {
	FVector currentCoord = endBlockCorrd;
	int count = 0;
	while (true) {
		count ++;
		if (abs(currentCoord.X - startBlockCorrd.X) <= 15 &&
		abs(currentCoord.Y - startBlockCorrd.Y) <= 15) {
			UE_LOG(LogTemp, Log, TEXT("back search found"))
			break;
		}
		if (count > ((xLen / width) * (yLen / width)))
		{
			return false;
		}
		FVector coordDirection = directions[currentCoord.Y / width][currentCoord.X / width];
		solvedResultCoords.Add(coordDirection);

		currentCoord.X -= coordDirection.X;
		currentCoord.Y -= coordDirection.Y;
		currentCoord.Z -= coordDirection.Z;
	}
	UE_LOG(LogTemp, Log, TEXT("backTrace Done!"))
	// GEngine->AddOnScreenDebugMessage(5, 60.f, FColor::Black, FString("backTrace Done!"));
	return true;
}

FVector AAStarPathfinding::getNextStepDirection() {
	FVector value = solvedResultCoords.Pop();
	// GEngine->AddOnScreenDebugMessage(6, 60.f, FColor::Black, FString("getNextStepDirection Done!"));
	return value;
}