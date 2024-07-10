// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyTowerStat.h"
#include "MyTowerEnemyStat.h"
#include "MyGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMySingleTon, Error, All);

/**
 * 
 */
UCLASS()
class PORT_API UMyGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UMyGameSingleton();
	static UMyGameSingleton& Get();

public:
	FORCEINLINE const FMyTowerStat GetTowerStat(int32 Level) const { return TowerStatTable.IsValidIndex(Level-1) ? TowerStatTable[Level-1] : FMyTowerStat(); }
	FORCEINLINE const FMyTowerEnemyStat GetEnemyStat(int32 Level) const { return EnemyStatTable.IsValidIndex(Level-1) ? EnemyStatTable[Level-1] : FMyTowerEnemyStat(); }

	UPROPERTY()
	int32 TowerMaxLevel;

	UPROPERTY()
	int32 EnemyMaxLevel;

private:
	TArray<FMyTowerStat> TowerStatTable;
	TArray<FMyTowerEnemyStat> EnemyStatTable;
	
};
