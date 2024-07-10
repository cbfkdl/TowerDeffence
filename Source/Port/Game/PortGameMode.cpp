// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/PortGameMode.h"
#include "Character/MyCharacterBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "MySpawnVolume.h"
#include "EngineUtils.h"
#include "Engine.h"

APortGameMode::APortGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

// 
	static ConstructorHelpers::FClassFinder<AMyPlayerController> PlayerConrtollerRef(TEXT("/Script/CoreUObject.Class'/Script/Port.MyPlayerController'"));
	if (PlayerConrtollerRef.Class)
	{
		PlayerControllerClass = PlayerConrtollerRef.Class;
	}

	// 기본 감소 비율
	DecreaseRate = 0.01f;

}

void APortGameMode::BeginPlay()
{
	Super::BeginPlay();

	// find all spawn volume actors
	/*
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMySpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		AMySpawnVolume* SpawnVolumeActor = Cast<AMySpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.Add(SpawnVolumeActor);
		}
	}
	*/
	// GetAllActorsOfClass 블루프린트에서 주로 사용하는 함수
	for (TActorIterator<AMySpawnVolume> it(GetWorld()); it; ++it)
	{
		SpawnVolumeActors.Add(*it);
	}

	SetCurrentState(EBatteryPlayState::EPlaying);
	
	AMyCharacterBase* MyCharacter = Cast<AMyCharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		PowerToWin = MyCharacter->GetInitialPower() * 1.25f;
	}
}

void APortGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMyCharacterBase* MyCharacter = Cast<AMyCharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		if (MyCharacter->GetCharacterPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::EWon);
		}
		else if (MyCharacter->GetCharacterPower() > 0)
		{
			// CharacterPower DecreaseRate 만큼 감소
			MyCharacter->UpdatePower(-DeltaTime * DecreaseRate * (MyCharacter->GetInitialPower()));
		}
		else
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}

	}

}

float APortGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

void APortGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void APortGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState)
	{
	case EBatteryPlayState::EPlaying:
	{
		for (auto Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(true);
		}
	}
	break;
	case EBatteryPlayState::EGameOver:
	{
		for (auto Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}

		ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (MyCharacter)
		{
			//ragdoll
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}
	}
	break;
	case EBatteryPlayState::EWon:
	{
		for (auto Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
	}
	break;
	case EBatteryPlayState::EUnknown:
	default:
	{
	}
	break;
	}
}
