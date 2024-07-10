// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TowerEnemySpawnActor.h"
#include "Character/Dragon/DragonNonCharacter.h"
#include "Game/ColorGameModeBase.h"

// Sets default values
ATowerEnemySpawnActor::ATowerEnemySpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentCount = 0;
}

// Called when the game starts or when spawned
void ATowerEnemySpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATowerEnemySpawnActor::SetSpawningActive(bool bActive)
{
	if (bActive)
	{
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATowerEnemySpawnActor::SpawnEnemy, SpawnDelay, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		CurrentCount = 0;
	}
}

void ATowerEnemySpawnActor::SpawnEnemy()
{
	++CurrentCount;
	if (CurrentCount > MaxSpawnCount)
	{
		SetSpawningActive(false);
		return;
	}

	if (IsValid(WhatToSpawn))
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			// Spawn
			ADragonNonCharacter* const SpawnedActor = World->SpawnActor<ADragonNonCharacter>(WhatToSpawn, SpawnLocation, FRotator::ZeroRotator, FActorSpawnParameters());
			AColorGameModeBase* pGameMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
			if (IsValid(pGameMode))
			{
				SpawnedActor->InitRoundStat(pGameMode->GetCurrentRound());
			}

			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATowerEnemySpawnActor::SpawnEnemy, SpawnDelay, false);
		}
	}
}
