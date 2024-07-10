// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyCharacterStatWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PortGameMode.h"

void UMyCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FMyCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%s"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookUp.Add(PropKey, BaseTextBlock);
		}
	}

	// hard code
	const FName PropKey(TEXT("TxtPowerToWin"));
	UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtPowerToWin")));
	if (BaseTextBlock)
	{
		BaseLookUp.Add(PropKey, BaseTextBlock);
	}
}

void UMyCharacterStatWidget::UpdatePowerToWin()
{
	AGameModeBase* MyMode = UGameplayStatics::GetGameMode(GetWorld());
	APortGameMode* PortMode = Cast<APortGameMode>(MyMode);
	if (PortMode)
	{
		const FName NameKey(FString::Printf(TEXT("TxtPowerToWin")));
		float PowerToWin = PortMode->GetPowerToWin();
		UE_LOG(LogClass, Log, TEXT("PowerToWin %s"), *NameKey.ToString());

		FString FormatSpec = *FString::Printf(TEXT("%.2f"), PowerToWin);
		UTextBlock** BaseTextBlockPtr = BaseLookUp.Find(NameKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FormatSpec));
		}
	}
}

void UMyCharacterStatWidget::UpdateStat(const FMyCharacterStat& Stat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FMyCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&Stat, &BaseData);
		
		FString FormatSpec= *FString::Printf(TEXT("%.2f"), BaseData);
		
		UTextBlock** BaseTextBlockPtr = BaseLookUp.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FormatSpec));
		}
	}
}
