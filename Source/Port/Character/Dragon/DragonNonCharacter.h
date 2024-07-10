// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Dragon/DragonCharacter.h"
#include "Interface/MyEnemyStatInterface.h"
#include "DragonNonCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API ADragonNonCharacter : public ADragonCharacter, public IMyEnemyStatInterface
{
	GENERATED_BODY()
	
public:
	ADragonNonCharacter();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameInfo)
	int DestroyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameInfo)
	int Score;

public:
	void InitRoundStat(int Round);

protected:
	virtual void SetDead() override;
	virtual void BeginPlay() override;

public:
	virtual int GetScore() override;
	virtual int GetDamage() override;
	//virtual void UpdateEnemyCount() override;

};
