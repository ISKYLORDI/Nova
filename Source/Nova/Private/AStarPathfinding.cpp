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


void AAStarPathfinding::updateQueueCoords(int gDistance, AActor* selfActor, AActor* playerActor, TArray<AActor*> otherNPCActor) {
	TArray<FVector> dirs = {FVector(width, 0, 0),FVector(-width, 0, 0)
		,FVector(0, width, 0),FVector(0, -width, 0)};
	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Black, FString::FromInt(searchedCoords.Num()));
	TArray<AActor*> AllIgnore = otherNPCActor;
	AllIgnore.Append(TArray<AActor*>({selfActor, playerActor}));
	
	for (FVector coord : searchedCoords) {
		for (FVector dir : dirs) {
			FVector tempVector = coord;
			tempVector.X += dir.X;
			tempVector.Y += dir.Y;
			if (
			tempVector.X >= xLen ||
			tempVector.X < 0 ||
			tempVector.Y >= yLen ||
			tempVector.Y < 0
			) {
				UE_LOG(LogTemp, Log, TEXT("passed"))
				continue;
			}
			FVector Start = FVector(coord.X + width / 2, coord.Y + width / 2, coord.Z);
			FVector End = FVector(tempVector.X + width / 2, tempVector.Y + width / 2, tempVector.Z);
			// GEngine->AddOnScreenDebugMessage(9, 60.f, FColor::Black, Start.ToString());
			// GEngine->AddOnScreenDebugMessage(10, 60.f, FColor::Black,  End.ToString());

			// FRONT
			FHitResult BoxHit;
			if (UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,																																																		
			End,
			FVector(5.f , 5.f, 10.f),
			this->GetActorRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			AllIgnore,
			EDrawDebugTrace::None,
			BoxHit,
			true))
			{
				// UP 扫描头顶一层，可以通过则可以攀爬

				if (UKismetSystemLibrary::BoxTraceSingle(
					this,
					FVector(coord.X + width / 2, coord.Y + width / 2, coord.Z + width),
					FVector(tempVector.X + width / 2, tempVector.Y + width / 2, tempVector.Z + width),
					FVector(5.f , 5.f, 10.f),
					this->GetActorRotation(),
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					AllIgnore,
					EDrawDebugTrace::None,
					BoxHit,
					true))
				{
					continue;
				}
				
				dir.Z = width;
				
			}
			// Down
			// scan 1 前面往下扫描一格
			if (!UKismetSystemLibrary::BoxTraceSingle(
			this,
			FVector(tempVector.X + width / 2, tempVector.Y + width / 2, coord.Z),
			FVector(tempVector.X + width / 2, tempVector.Y + width / 2, tempVector.Z - width),
			FVector(5.f , 5.f, 10.f),
			this->GetActorRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			AllIgnore,
			EDrawDebugTrace::None,
			BoxHit,
			true))
			{
				// 如果没有扫描到，继续扫描一格
				if (!UKismetSystemLibrary::BoxTraceSingle(
					this,
					FVector(tempVector.X + width / 2, tempVector.Y + width / 2, coord.Z- width),
					FVector(tempVector.X + width / 2, tempVector.Y + width / 2, tempVector.Z - width * 2),
					FVector(5.f , 5.f, 10.f),
					this->GetActorRotation(),
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					AllIgnore,
					EDrawDebugTrace::None,
					BoxHit,
					true))
				{
					// 依然没有陆地，无法通过
					continue;
				}
				// 拥有路段 下降一各
				dir.Z = -width;
			}
			if (!queueCoords.Contains(tempVector) && !searchedCoords.Contains(tempVector)) {
				gDistances[tempVector.Y / width][tempVector.X / width] = gDistance;
				directions[tempVector.Y / width][tempVector.X / width] = dir;
				queueCoords.Add(tempVector);
			}
			
		}
	}
	// GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Black, FString("updateQueueCoords Done!"));
}


void AAStarPathfinding::solve( AActor* selfActor, AActor* playerActor, TArray<AActor*> otherNPCActor) {
	FVector nextStepCoord = startBlockCorrd;
	
	int currentGDistance = width;
	
	while (true) {
		float leastFDistance = 10000000;
		float leastHDistance = 10000000;

		searchedCoords.Add(nextStepCoord);
		queueCoords.Remove(nextStepCoord);
		updateQueueCoords(currentGDistance, selfActor, playerActor, otherNPCActor);
		// UE_LOG(LogTemp, Log, TEXT("ignore actor num: %d"), IgnoreWhenTracing.Num())
		
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
		
		if (abs(nextStepCoord.X - endBlockCorrd.X) <= 10 &&
		abs(nextStepCoord.Y - endBlockCorrd.Y) <= 10) {
			break;
		}

		currentGDistance += width;
	}
	backTrace();
	// GEngine->AddOnScreenDebugMessage(5, 60.f, FColor::Black, FString("solve Done!"));
}

void AAStarPathfinding::backTrace() {
	FVector currentCoord = endBlockCorrd;
	while (true) {
		if (abs(currentCoord.X - startBlockCorrd.X) <= 10 &&
		abs(currentCoord.Y - startBlockCorrd.Y) <= 10) {
			break;
		}
		FVector coordDirection = directions[currentCoord.Y / width][currentCoord.X / width];
		solvedResultCoords.Add(coordDirection);
		
		currentCoord = FVector(currentCoord.X - coordDirection.X, currentCoord.Y - coordDirection.Y, zCoord - coordDirection.Z);
	}
	// GEngine->AddOnScreenDebugMessage(5, 60.f, FColor::Black, FString("backTrace Done!"));
}

FVector AAStarPathfinding::getNextStepDirection() {
	FVector value = solvedResultCoords.Pop();
	// GEngine->AddOnScreenDebugMessage(6, 60.f, FColor::Black, FString("getNextStepDirection Done!"));
	return value;
}