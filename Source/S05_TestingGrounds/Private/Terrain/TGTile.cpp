// Fill out your copyright notice in the Description page of Project Settings.
#include "TGTile.h"


#include "ActorPool.h"
#include "AI/NavigationSystemBase.h"

// Sets default values
ATGTile::ATGTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NavigationBoundsOffset = FVector(2000, 0, 0);

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
}


// Called when the game starts or when spawned
void ATGTile::BeginPlay()
{
	Super::BeginPlay();
}


void ATGTile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Pool != nullptr && NavMeshBoundsVolume != nullptr)
	{
		Pool->Return(NavMeshBoundsVolume);
	}
}


// Called every frame
void ATGTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ATGTile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()))

	Pool = InPool;
	PositionNavMeshBoundsVolume();
}


void ATGTile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();

	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName())
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("[%s] Checked out: %s"), *GetName(), *NavMeshBoundsVolume->GetName())
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	FNavigationSystem::Build(*GetWorld());
}


template<typename T>
void ATGTile::RandomlyPlaceActors(const TSubclassOf<T>& ToSpawn, const FRandSpawnPosition& RandomPosition)
{
	int	NumberToSpawn = FMath::RandRange(RandomPosition.MinSpawn, RandomPosition.MaxSpawn);

	for (int32 i = 0; i < NumberToSpawn; ++i)
	{
		FSpawnPosition SpawnPosition;

		SpawnPosition.Scale = FMath::RandRange(RandomPosition.MinScale, RandomPosition.MaxScale);
		bool bIsFoundLocation = FindEmptyLocation(OUT SpawnPosition.Location, RandomPosition.Radius * SpawnPosition.Scale);

		if (bIsFoundLocation)
		{
			SpawnPosition.Rotation = FMath::FRandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}


void ATGTile::PlaceActors(const TSubclassOf<AActor> ToSpawn, FRandSpawnPosition RandomPosition)
{
	RandomlyPlaceActors(ToSpawn, RandomPosition);
}


void ATGTile::PlaceAISpawns(const TSubclassOf<APawn> ToSpawn, FRandSpawnPosition RandomPosition)
{
	RandomlyPlaceActors(ToSpawn, RandomPosition);
}


void ATGTile::PlaceActor(const TSubclassOf<AActor>& ToSpawn, const FSpawnPosition& SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);

	if (Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}


void ATGTile::PlaceActor(const TSubclassOf<APawn>& ToSpawn, const FSpawnPosition& SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);

	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);
	
	if (Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}
}


bool ATGTile::FindEmptyLocation(FVector& OutLocation, const float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);

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

	return !bHasHit;
}
