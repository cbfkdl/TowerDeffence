// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* Pawn) override;

public:
	void SetNextPos();


private:
	int PosIndex;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
