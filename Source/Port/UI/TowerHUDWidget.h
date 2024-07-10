// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API UTowerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTowerHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateRound(int Round);
	void UpdateHp(int Hp);
	void UpdateGold(int Gold);
	void UpdateCountdown(int Count);


private:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UMyTowerStatWidget> TowerGameStat;

	UPROPERTY()
	TObjectPtr<class UMyCountDownWidget> Countdown;
};
