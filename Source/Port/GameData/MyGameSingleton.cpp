// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/MyGameSingleton.h"

DEFINE_LOG_CATEGORY(LogMySingleTon);

UMyGameSingleton::UMyGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TowerDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Gamedata/TowerStatTable.TowerStatTable'"));
	if(TowerDataTableRef.Object)
	{
		const UDataTable* DataTable = TowerDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, TowerStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FMyTowerStat*>(Value);
			}
		);
	}

	TowerMaxLevel = TowerStatTable.Num();
	ensure(TowerMaxLevel > 0);

	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Gamedata/TowerEnemyStatTable.TowerEnemyStatTable'"));
	if (EnemyDataTableRef.Object)
	{
		const UDataTable* DataTable = EnemyDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, EnemyStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FMyTowerEnemyStat*>(Value);
			}
		);
	}


	EnemyMaxLevel = EnemyStatTable.Num();
	ensure(EnemyMaxLevel > 0);

}

UMyGameSingleton& UMyGameSingleton::Get()
{
	UMyGameSingleton* SingleTon = CastChecked<UMyGameSingleton>(GEngine->GameSingleton);
	if (SingleTon)
	{
		return *SingleTon;
	}

	UE_LOG(LogMySingleTon, Error, TEXT("Invalid Singleton"));
	return *NewObject<UMyGameSingleton>();
}
