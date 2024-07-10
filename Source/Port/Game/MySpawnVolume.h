// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySpawnVolume.generated.h"

UCLASS()
class PORT_API AMySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE class UBoxComponent* GetWhereToSapwn() const { return WhereToSpawn; }

	UFUNCTION(BlueprintPure, Category = Spawning)
	FVector GetRandomPointVolume();

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void SetSpawningActive(bool bActive);

protected:
	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf <class AMyItemBox> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float MinSpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Spawning)
	float MaxSpawnDelay;

	
private:
	// Box Component Spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawning, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

	// Handle Spawning new item
	void SpawnPickup();

	// current spawn delay
	float SpawnDelay;
};
