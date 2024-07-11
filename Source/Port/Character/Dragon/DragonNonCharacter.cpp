// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragon/DragonNonCharacter.h"
#include "AI/MyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/MyWidgetComponent.h"
#include "UI/MyHpBarWidget.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PortCollision.h"
#include "Game/ColorGameModeBase.h"
#include "CharacaterStat/MyTowerStatComponent.h"

ADragonNonCharacter::ADragonNonCharacter()
{
	//GetMesh()->SetHiddenInGame(false);

	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_CAPSULE);
	//GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	AttackType = EAttackType::ENone;
}

void ADragonNonCharacter::InitRoundStat(int Round)
{
	Stat->SetLevelStat(Round, false);
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetMaxMovementSpeed();
}

void ADragonNonCharacter::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			AColorGameModeBase* pGameMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
			if (IsValid(pGameMode))
			{
				pGameMode->UpdateEnemyCount();
			}
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void ADragonNonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HpBar->SetHiddenInGame(false);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_CAPSULE);

}

int ADragonNonCharacter::GetScore()
{
	return Score;
}

int ADragonNonCharacter::GetDamage()
{
	UE_LOG(LogClass, Log, TEXT("Damage : %d"), DestroyDamage);
	return DestroyDamage;
}
