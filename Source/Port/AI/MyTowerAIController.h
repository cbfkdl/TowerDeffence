// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyTowerAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API AMyTowerAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyTowerAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* Pawn) override;


private:
	int PosIndex;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
};
