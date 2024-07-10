// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ColorGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, const int /* Hp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundChangedDelegate, const int /* Round*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCountChangedDelegate, const int /* CountDown*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGoldChangedDelegate, const int /* CountDown*/);


// Enum
UENUM(BlueprintType)
enum class ETowerPlayState : uint8
{
	EWait = 0,
	ENextRound,
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS()
class PORT_API AColorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AColorGameModeBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// HUD용 
	FOnHpChangedDelegate OnHpChanged;
	FOnRoundChangedDelegate OnRoundChanged;
	FOnCountChangedDelegate OnCountChanged;
	FOnGoldChangedDelegate OnGoldChanged;

	UFUNCTION(BlueprintPure, Category = Power)
	ETowerPlayState GetCurrentState() const { return CurrentState; }

	void SetCurrentState(ETowerPlayState NewState);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	TArray<FVector> MonsterDestPosArr;

	void UpdateHP(int Hp);
	void UpdateEnemyCount();
	void SetGold(bool NextRound = false);
	bool IsValidBuild();
	FORCEINLINE int GetHp() const { return GameHp; }
	FORCEINLINE int GetRoundToWin() const { return RoundToWin; }
	FORCEINLINE int GetCurrentRound() const { return CurrentRound; }
	FORCEINLINE int GetCountDown() const { return CountDown; }
	FORCEINLINE int GetGold() const { return Gold; }
	FORCEINLINE int GetClearGold() const { return ClearGold; }


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Rule, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDWigetClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	// 승리에 필요한 라운드 수 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Rule, Meta = (AllowPrivateAccess = "true"))
	int RoundToWin;

	int CurrentRound;
	int CurrentCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rule, Meta = (AllowPrivateAccess = "true"))
	int GameHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rule, Meta = (AllowPrivateAccess = "true"))
	int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rule, Meta = (AllowPrivateAccess = "true"))
	int Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rule, Meta = (AllowPrivateAccess = "true"))
	int ClearGold;

	int CountDown;

	ETowerPlayState CurrentState;

	TArray<class ATowerEnemySpawnActor*> SpawnVolumeActors;

	// 게임 플레이 상태에 영향을 주는 모든 함수의 호출 담당
	void HandleNewState(ETowerPlayState NewState);

	void CountDownStart();
	void CountDownRound();

};
