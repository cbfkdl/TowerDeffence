// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacaterStat/MyTowerStatComponent.h"
#include "GameData/MyGameSingleton.h"

// Sets default values for this component's properties
UMyTowerStatComponent::UMyTowerStatComponent()
{
	CurrentLevel = 1;
}


// Called when the game starts
void UMyTowerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStat(CurrentLevel, false);
	SetLevelStat(CurrentLevel, true);

	SetHp(EnemyStat.MaxHp);
}

void UMyTowerStatComponent::SetLevelStat(int32 NewLevel, bool IsTower)
{
	if (IsTower)
	{
		CurrentLevel = FMath::Clamp(NewLevel, 1, UMyGameSingleton::Get().TowerMaxLevel);
		TowerStat = UMyGameSingleton::Get().GetTowerStat(CurrentLevel);
	}
	else
	{
		CurrentLevel = FMath::Clamp(NewLevel, 1, UMyGameSingleton::Get().EnemyMaxLevel);
		EnemyStat = UMyGameSingleton::Get().GetEnemyStat(CurrentLevel);
		SetHp(EnemyStat.MaxHp);

		check(EnemyStat.MaxHp > 0.0f);
	}
}

float UMyTowerStatComponent::ApplyDamage(float Damage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(Damage, 0, Damage);

	SetHp(PrevHp - ActualDamage);
	
	return ActualDamage;
}

void UMyTowerStatComponent::SetHp(float Hp)
{
	CurrentHp = FMath::Clamp<float>(Hp, 0.0f, EnemyStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}


