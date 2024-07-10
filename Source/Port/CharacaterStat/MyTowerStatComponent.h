// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MyTowerStat.h"
#include "GameData/MyTowerEnemyStat.h"
#include "MyTowerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChanged, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORT_API UMyTowerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyTowerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChanged OnHpChanged;

	// false면 에너미
	void SetLevelStat(int32 NewLevel, bool IsTower);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel;  }

	FORCEINLINE float GetMaxMovementSpeed() const { return EnemyStat.MovementSpeed; }
	FORCEINLINE float GetMaxHp() const { return EnemyStat.MaxHp; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	FORCEINLINE float GetAttack() const { return TowerStat.Attack; }
	FORCEINLINE float GetAttackRadius() const { return TowerStat.AttackRange; }
	FORCEINLINE float GetAttackSpeed() const { return TowerStat.AttackSpeed; }
	FORCEINLINE float GetLevelColor() const { return TowerStat.LevelBlueColor; }
	float ApplyDamage(float Damage);

protected:
	void SetHp(float Hp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FMyTowerStat TowerStat;

	UPROPERTY(Transient ,VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FMyTowerEnemyStat EnemyStat;

};
