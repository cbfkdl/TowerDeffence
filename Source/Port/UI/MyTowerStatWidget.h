// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTowerStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API UMyTowerStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateRound(int iRound);
	void UpdateHp(int iHp);
	void UpdateGold(int iGold);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookUp;
};
