// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/MyCharacterWidgetInterface.h"


UMyHpBarWidget::UMyHpBarWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UMyHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName("PbHpBar"));
	ensure(HpProgressBar);

	IMyCharacterWidgetInterface* CharacterWidget = Cast<IMyCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UMyHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
