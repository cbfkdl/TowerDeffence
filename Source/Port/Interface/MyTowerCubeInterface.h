// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyTowerCubeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyTowerCubeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORT_API IMyTowerCubeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 큐브 색상 변경
	virtual void ChangeColor(FVector Color = FVector(1.0f, 1.0f, 1.0f), float Alpha = 1.0f) = 0;
	virtual void SetTurret(bool OnOff) = 0;
	virtual bool GetIsOnTurret() = 0;
	virtual void SetOnTower(class ADragonCharacter* _Tower) = 0;
	virtual class ADragonCharacter* GetOnTower() = 0;
	virtual enum class EAttackType GetAttackType() = 0;

};
