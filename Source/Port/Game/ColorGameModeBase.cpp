// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ColorGameModeBase.h"
#include "Player/MyTopDownController.h"
#include "Game/TowerEnemySpawnActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AColorGameModeBase::AColorGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Pawn/TopDownPawn.TopDownPawn_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AMyTopDownController> PlayerConrtollerRef(TEXT("/Script/CoreUObject.Class'/Script/Port.MyTopDownController'"));
	if (PlayerConrtollerRef.Class)
	{
		PlayerControllerClass = PlayerConrtollerRef.Class;
	}

	//csv로 불러들이는게 나을듯
	MonsterDestPosArr.Add(FVector(450.0f, 150.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(-450.0f, 150, 0.0f));
	MonsterDestPosArr.Add(FVector(-450.0f, 450.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(-150.0f, 450.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(-150.0f, -450.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(-450.0f, -450.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(-450.0f, -150.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(450.0f, -150.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(450.0f, -450.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(150.0f, -450.0f, 0.0f));
	MonsterDestPosArr.Add(FVector(150.0f, 450.0f, 0.0f));

	UE_LOG(LogClass, Log, TEXT("Init Hp : %d"), GameHp);

	CountDown = 3;
	CurrentRound = 1;
	RoundToWin = 5;
}

void AColorGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ATowerEnemySpawnActor> it(GetWorld()); it; ++it)
	{
		SpawnVolumeActors.Add(*it);
	}

	SetCurrentState(ETowerPlayState::EWait);
}

void AColorGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColorGameModeBase::SetCurrentState(ETowerPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void AColorGameModeBase::UpdateHP(int Hp)
{
	GameHp -= Hp;

	OnHpChanged.Broadcast(GameHp);

	if (GameHp < 1)
	{
		HandleNewState(ETowerPlayState::EGameOver);
	}
	//else if()
}

void AColorGameModeBase::UpdateEnemyCount()
{
	CurrentCount -= 1;
	if (CurrentCount <= 0)
	{
		HandleNewState(ETowerPlayState::ENextRound);
	}
}

bool AColorGameModeBase::IsValidBuild()
{
	if(Gold < Cost)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void AColorGameModeBase::SetGold(bool NextRound/* = false*/)
{
	if(!NextRound)
	{
		Gold -= Cost;
	}
	else
	{
		Gold += ClearGold;
	}
	OnGoldChanged.Broadcast(Gold);
}


void AColorGameModeBase::HandleNewState(ETowerPlayState NewState)
{
	switch (NewState)
	{
	case ETowerPlayState::EWait:
	{
		CountDownStart();
	}
	break;
	case ETowerPlayState::ENextRound:
	{
		SetGold(true);
		CountDownRound();
	}
	break;
	case ETowerPlayState::EPlaying:
	{
		for (auto Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(true);
			CurrentCount = Volume->GetMaxSpawnCount();
		}
		
	}
	break;
	case ETowerPlayState::EGameOver:
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
		// 임의로 -1은 승리 -2는 게임오버
		OnCountChanged.Broadcast(-2);
		// 게임오버 메시지
	}
	break;
	case ETowerPlayState::EWon:
	{
		for (auto Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
		OnCountChanged.Broadcast(-1);
		UE_LOG(LogClass, Log, TEXT("Win"));
	}
	break;
	case ETowerPlayState::EUnknown:
	default:
	{
	}
	break;
	}
}

void AColorGameModeBase::CountDownStart()
{
	OnCountChanged.Broadcast(CountDown);
	--CountDown;
	if(CountDown > -1)
	{
		FTimerHandle CountTimer;
		GetWorldTimerManager().SetTimer(CountTimer, this, &AColorGameModeBase::CountDownStart, 1.0f, false);
	}
	else
	{
		HandleNewState(ETowerPlayState::EPlaying);
		CountDown = 3;
	}
}

void AColorGameModeBase::CountDownRound()
{
	--CountDown;
	if (CountDown > -1)
	{
		FTimerHandle CountTimer;
		GetWorldTimerManager().SetTimer(CountTimer, this, &AColorGameModeBase::CountDownRound, 1.0f, false);
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("CountDownRound"));

		HandleNewState(ETowerPlayState::EPlaying);
		CountDown = 3;
		++CurrentRound;
		if (CurrentRound > RoundToWin)
		{
			CurrentRound -= 1;
			HandleNewState(ETowerPlayState::EWon);
			return;
		}
		OnRoundChanged.Broadcast(CurrentRound);

	}
}
