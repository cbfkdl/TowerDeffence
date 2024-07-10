// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/MyCharacterStat.h"
#include "MyHUDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API UMyHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMyHUDUserWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdatePowerToWin();
	void UpdateStat(const FMyCharacterStat& Stat);


private:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UMyHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UMyCharacterStatWidget> CharacterStat;
};
