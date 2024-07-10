// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "Interface/MyTowerAllInterface.h"
#include "Physics/PortCollision.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(OwnerPawn) == false)
	{
		return false;
	}

	IMyTowerAllInterface* AIPawn = Cast<IMyTowerAllInterface>(OwnerPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	if (IsValid(Target) == false)
	{
		return false;
	}

	float DistanceToTarget = OwnerPawn->GetDistanceTo(Target);
	float DetectRadius = AIPawn->GetAIDetectRange();
	Result = (DistanceToTarget <= DetectRadius);

	if (!Result)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
	}

	return Result;
}
