// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGTile.generated.h"


USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

public:
	FSpawnPosition()
	{ }

	FSpawnPosition(FVector _Location, float _Rotation, float _Scale) :
		Location(_Location), Rotation(_Rotation), Scale(_Scale)
	{ }

public:
	FVector Location;
	float Rotation;
	float Scale;
};


USTRUCT(BlueprintType)
struct FRandSpawnPosition
{
	GENERATED_USTRUCT_BODY()

public:
	FRandSpawnPosition() :
		Radius(500), MinSpawn(1), MaxSpawn(1), MinScale(1), MaxScale(1)
	{ }

public:
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	float Radius;

	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	int32 MinSpawn;

	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	int32 MaxSpawn;

	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	float MinScale;

	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	float MaxScale;
};


UCLASS()
class S05_TESTINGGROUNDS_API ATGTile : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ATGTile();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(class UActorPool* InPool);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void PlaceActors(const TSubclassOf<AActor> ToSpawn, FRandSpawnPosition RandomPosition);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void PlaceAISpawns(const TSubclassOf<APawn> ToSpawn, FRandSpawnPosition RandomPosition);


private:
	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;


private:
	void PositionNavMeshBoundsVolume();

	TArray<FSpawnPosition> RandomSpawnPositions(FRandSpawnPosition& RandomPosition);

	template<typename T>
	void RandomlyPlaceActors(const TSubclassOf<T>& ToSpawn, const FRandSpawnPosition& RandomPosition);

	void PlaceActor(const TSubclassOf<AActor>& ToSpawn, const FSpawnPosition& SpawnPosition);

	void PlaceActor(const TSubclassOf<APawn> &ToSpawn, const FSpawnPosition &SpawnPosition);

	bool FindEmptyLocation(FVector& OutLocation, const float Radius);

	bool CanSpawnAtLocation(const FVector Location, const float Radius);
};
