// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGTile.generated.h"

UCLASS()
class S05_TESTINGGROUNDS_API ATGTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATGTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceActors(const TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius = 500);

private:
	void PlaceActor(const TSubclassOf<AActor>& ToSpawn, FVector SpawnPoint);

	bool FindEmptyLocation(FVector& OutLocation, const float Radius);

	bool CanSpawnAtLocation(const FVector Location, const float Radius);
};
