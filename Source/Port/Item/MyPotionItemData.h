// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MyItemDataAsset.h"
#include "Game/MyCharacterStat.h"
#include "MyPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API UMyPotionItemData : public UMyItemDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("MyItemDataAsset", GetFName());
	}

	UMyPotionItemData();

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FMyCharacterStat BaseStat;

};
