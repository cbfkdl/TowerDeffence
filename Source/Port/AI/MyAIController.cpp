// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/ColorGameModeBase.h"

AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/CubeCollect/AI/BB_Character.BB_Character'"));
	if (IsValid(BBAssetRef.Object))
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/CubeCollect/AI/BT_TowerNpc.BT_TowerNpc'"));
	if (IsValid(BTAssetRef.Object))
	{
		BTAsset = BTAssetRef.Object;
	}

	PosIndex = 0;
}

void AMyAIController::RunAI()
{
	UBlackboardComponent* pBlackboard = Blackboard.Get();
	AColorGameModeBase* pGmaeMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
	if (UseBlackboard(BBAsset, pBlackboard))
	{
		if (IsValid(pGmaeMode) && pGmaeMode->MonsterDestPosArr.Num() > 0)
		{
			FVector Start = pGmaeMode->MonsterDestPosArr[PosIndex];
			Blackboard->SetValueAsVector("NextPos", Start);
		}
		

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AMyAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BTComponent))
	{
		BTComponent->StopTree();
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void AMyAIController::SetNextPos()
{
	++PosIndex;

	UBlackboardComponent* pBlackboard = Blackboard.Get();
	AColorGameModeBase* pGmaeMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
	if (UseBlackboard(BBAsset, pBlackboard))
	{
		int Count = pGmaeMode->MonsterDestPosArr.Num();
		if (Count > 0 && PosIndex < Count)
		{
			FVector Next = pGmaeMode->MonsterDestPosArr[PosIndex];
			Blackboard->SetValueAsVector("NextPos", Next);
		}
		else
		{
			//UE_LOG(LogClass, Log, TEXT("Last Destination"));
		}
	}
}
