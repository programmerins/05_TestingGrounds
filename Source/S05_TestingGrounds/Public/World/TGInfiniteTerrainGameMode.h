// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
#include "World/SGameMode.h"
#include "TGInfiniteTerrainGameMode.generated.h"


class UActorPool;


/**
 * 
 */
UCLASS()
class S05_TESTINGGROUNDS_API ATGInfiniteTerrainGameMode : public ASGameMode
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	UActorPool* NavMeshBoundsVolumePool;


public:
	ATGInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();


private:
	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);
};
