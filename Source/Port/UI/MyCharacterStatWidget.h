// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/MyCharacterStat.h"
#include "MyCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API UMyCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdatePowerToWin();
	void UpdateStat(const FMyCharacterStat& Stat);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookUp;
};
