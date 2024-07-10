// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyTowerAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/ColorGameModeBase.h"

AMyTowerAIController::AMyTowerAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/CubeCollect/AI/BB_Tower.BB_Tower'"));
	if (IsValid(BBAssetRef.Object))
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/CubeCollect/AI/BT_Tower.BT_Tower'"));
	if (IsValid(BTAssetRef.Object))
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AMyTowerAIController::RunAI()
{
	UBlackboardComponent* pBlackboard = Blackboard.Get();
	AColorGameModeBase* pGmaeMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
	if (UseBlackboard(BBAsset, pBlackboard))
	{
		if (pGmaeMode)
		{
			Blackboard->SetValueAsObject("Target", nullptr);
		}


		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AMyTowerAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BTComponent))
	{
		BTComponent->StopTree();
	}
}

void AMyTowerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
