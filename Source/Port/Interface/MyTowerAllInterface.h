// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyTowerAllInterface.generated.h"

DECLARE_DELEGATE(FAITowerAttackFinished);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyTowerAllInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORT_API IMyTowerAllInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIDetectRange() = 0;

	virtual void SetAIAttackDelegate(const FAITowerAttackFinished& OnAttackFinished) = 0;
	virtual void AttackByAI(APawn* InTarget) = 0;
	
};
