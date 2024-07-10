// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUDUserWidget.h"
#include "Interface/MyCharacterHUDInterface.h"
#include "UI/MyCharacterStatWidget.h"

UMyHUDUserWidget::UMyHUDUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMyHUDUserWidget::UpdatePowerToWin()
{
	UE_LOG(LogClass, Log, TEXT("UpdatePowerToWin"));

	CharacterStat->UpdatePowerToWin();
}

void UMyHUDUserWidget::UpdateStat(const FMyCharacterStat& Stat)
{
	CharacterStat->UpdateStat(Stat);
}

void UMyHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UMyHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UMyCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	IMyCharacterHUDInterface* HUDPawn = Cast<IMyCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWIdget(this);
	}

}
