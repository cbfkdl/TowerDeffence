// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyTowerStatWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ColorGameModeBase.h"

void UMyTowerStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// hard code
	{
		const FName PropKey(TEXT("TxtRound"));
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtRound")));
		if (BaseTextBlock)
		{
			BaseLookUp.Add(PropKey, BaseTextBlock);
		}
	}

	{
		const FName PropKey(TEXT("TxtHp"));
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHp")));
		if (BaseTextBlock)
		{
			BaseLookUp.Add(PropKey, BaseTextBlock);
		}
	}

	{
		const FName PropKey(TEXT("TxtGold"));
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtGold")));
		if (BaseTextBlock)
		{
			BaseLookUp.Add(PropKey, BaseTextBlock);
		}
	}
}

void UMyTowerStatWidget::UpdateRound(int iRound)
{
	FName NameKey(FString::Printf(TEXT("TxtRound")));

	FString FormatSpec = *FString::Printf(TEXT("%d"), iRound);
	UTextBlock** BaseTextBlockPtr = BaseLookUp.Find(NameKey);
	if (BaseTextBlockPtr)
	{
		(*BaseTextBlockPtr)->SetText(FText::FromString(FormatSpec));
	}
}

void UMyTowerStatWidget::UpdateHp(int iHp)
{
	FName NameKey(FString::Printf(TEXT("TxtHp")));

	FString FormatSpec = *FString::Printf(TEXT("%d"), iHp);
	UTextBlock** BaseTextBlockPtr = BaseLookUp.Find(NameKey);
	if (BaseTextBlockPtr)
	{
		(*BaseTextBlockPtr)->SetText(FText::FromString(FormatSpec));
	}
}

void UMyTowerStatWidget::UpdateGold(int iGold)
{
	FName NameKey(FString::Printf(TEXT("TxtGold")));

	FString FormatSpec = *FString::Printf(TEXT("%d"), iGold);
	UTextBlock** BaseTextBlockPtr = BaseLookUp.Find(NameKey);
	if (BaseTextBlockPtr)
	{
		(*BaseTextBlockPtr)->SetText(FText::FromString(FormatSpec));
	}
}
