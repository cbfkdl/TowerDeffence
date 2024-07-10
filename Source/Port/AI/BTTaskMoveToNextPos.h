// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskMoveToNextPos.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API UBTTaskMoveToNextPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskMoveToNextPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
