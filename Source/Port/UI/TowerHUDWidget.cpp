// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TowerHUDWidget.h"
#include "Interface/MyTowerHUDInterface.h"
#include "UI/MyTowerStatWidget.h"
#include "UI/MyCountDownWidget.h"

UTowerHUDWidget::UTowerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTowerHUDWidget::UpdateRound(int Round)
{
	TowerGameStat->UpdateRound(Round);
}

void UTowerHUDWidget::UpdateHp(int Hp)
{
	TowerGameStat->UpdateHp(Hp);
}

void UTowerHUDWidget::UpdateGold(int Gold)
{
	TowerGameStat->UpdateGold(Gold);

}

void UTowerHUDWidget::UpdateCountdown(int Count)
{
	Countdown->UpdateCountDown(Count);
}

void UTowerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TowerGameStat = Cast<UMyTowerStatWidget>(GetWidgetFromName(TEXT("WidgetTowerStat")));
	ensure(TowerGameStat);

	Countdown = Cast<UMyCountDownWidget>(GetWidgetFromName(TEXT("Timer_Widget")));
	ensure(Countdown);

	IMyTowerHUDInterface* HUDPawn = Cast<IMyTowerHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWIdget(this);
	}
}


