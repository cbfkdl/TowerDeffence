// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerEnemySpawnActor.generated.h"

UCLASS()
class PORT_API ATowerEnemySpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerEnemySpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

public:
	void SetSpawningActive(bool bActive);
	int GetMaxSpawnCount() const { return MaxSpawnCount;  }

protected:
	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf <class ADragonNonCharacter> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float SpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	int MaxSpawnCount;

private:
	int CurrentCount;
	void SpawnEnemy();

};
