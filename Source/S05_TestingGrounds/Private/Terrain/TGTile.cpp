// Fill out your copyright notice in the Description page of Project Settings.

#include "TGTile.h"


#include "DrawDebugHelpers.h"


// Sets default values
ATGTile::ATGTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}


// Called when the game starts or when spawned
void ATGTile::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ATGTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATGTile::PlaceActors(const TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	int	NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (int32 i = 0; i < NumberToSpawn; ++i)
	{
		FVector SpawnPoint;
		bool bIsFoundLocation = FindEmptyLocation(OUT SpawnPoint, Radius);

		if (bIsFoundLocation)
		{
			PlaceActor(ToSpawn, SpawnPoint);
		}
	}
}


void ATGTile::PlaceActor(const TSubclassOf<AActor>& ToSpawn, FVector SpawnPoint)
{
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);

	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}


bool ATGTile::FindEmptyLocation(FVector& OutLocation, const float Radius)
{
	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	FBox Bounds(Min, Max);

	const int MAX_ATTEMPTS = 100;

	for (size_t i = 0; i < MAX_ATTEMPTS; ++i)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}

	return false;
}


bool ATGTile::CanSpawnAtLocation(const FVector Location, const float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool bHasHit = GetWorld()->SweepSingleByChannel
	(
		OUT HitResult,
		GlobalLocation, GlobalLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = bHasHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
	
	return !bHasHit;
}
