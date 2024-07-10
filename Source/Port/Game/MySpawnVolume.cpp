// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Item/MyItemBox.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMySpawnVolume::AMySpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Box Component to spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	MinSpawnDelay = 1.0f;
	MaxSpawnDelay = 4.5f;

	WhereToSpawn->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AMySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMySpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AMySpawnVolume::GetRandomPointVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void AMySpawnVolume::SetSpawningActive(bool bActive)
{
	if (bActive)
	{
		SpawnDelay = FMath::FRandRange(MinSpawnDelay, MaxSpawnDelay);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AMySpawnVolume::SpawnPickup, SpawnDelay, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

void AMySpawnVolume::SpawnPickup()
{
	if (WhatToSpawn)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			// Instigator = AActor Defined
			SpawnParams.Instigator = GetInstigator();

			// Get a Random location to Spawn 
			FVector SpawnLocation = GetRandomPointVolume();

			// Get a random Rotation 
			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			// Spawn
			AMyItemBox* const SpawnedBox = World->SpawnActor<AMyItemBox>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::FRandRange(MinSpawnDelay, MaxSpawnDelay);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AMySpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

