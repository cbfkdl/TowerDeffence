// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskMoveToNextPos.h"
#include "AI/MyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskMoveToNextPos::UBTTaskMoveToNextPos()
{
}

EBTNodeResult::Type UBTTaskMoveToNextPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(OwnerPawn) == false)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(OwnerPawn->GetWorld());
	if (IsValid(NavSystem) == false)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("NextPos")));
	float fDist = FVector::Dist(OwnerPawn->GetActorLocation(), Origin);
	if(fDist < 50.0f)
	{
		//UE_LOG(LogClass, Log, TEXT("fDist > 50"));
		AMyAIController* MyController = Cast<AMyAIController>(OwnerComp.GetAIOwner());
		if (MyController)
		{
			MyController->SetNextPos();
		}
	}

	FNavLocation NextDestPos;

	return Result;
}
