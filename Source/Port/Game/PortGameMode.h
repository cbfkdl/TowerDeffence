// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortGameMode.generated.h"

// Enum
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	EPlaying = 0,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class APortGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APortGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// HUD용 
	UFUNCTION(BlueprintPure, Category = Power)
	float GetPowerToWin() const;

	UFUNCTION(BlueprintPure, Category = Power)
	EBatteryPlayState GetCurrentState() const { return CurrentState; }

	void SetCurrentState(EBatteryPlayState NewState);

protected:
	// 배터리 파워가 줄어드는 비율
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Power, Meta = (AllowPrivateAccess = "true"))
	float DecreaseRate;
	
	// 승리에 필요한 파워
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Power, Meta = (AllowPrivateAccess = "true"))
	float PowerToWin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Power, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDWigetClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	EBatteryPlayState CurrentState;

	TArray<class AMySpawnVolume*> SpawnVolumeActors;

	// 게임 플레이 상태에 영향을 주는 모든 함수의 호출 담당
	void HandleNewState(EBatteryPlayState NewState);
};



