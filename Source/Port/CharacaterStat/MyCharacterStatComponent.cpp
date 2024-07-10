// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacaterStat/MyCharacterStatComponent.h"

// Sets default values for this component's properties
UMyCharacterStatComponent::UMyCharacterStatComponent()
{
	MaxHp = 200.0f;
	SetHp(MaxHp);

	bWantsInitializeComponent = true;
}

void UMyCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	BaseStat.MaxHp = MaxHp;
	SetHp(MaxHp / 2.0f);

	BaseStat.CharacterPower = 2000.0f;
	InitEnergy(BaseStat.CharacterPower);

	// 파워 레벨에 따른 속도 설정
	BaseStat.SpeedFactor = 0.75f;
	BaseStat.BaseSpeed = 10.0f;
}

float UMyCharacterStatComponent::GetCalculateWalkSpeed()
{
	return BaseStat.BaseSpeed + BaseStat.SpeedFactor * BaseStat.CharacterPower;
}

void UMyCharacterStatComponent::UpdateHp(float InHp)
{
	// Hp = Energy
	const float PrevHp = CurrentHp;
	const float ActualEnergy = FMath::Clamp<float>(InHp, 0, InHp);

	SetHp(PrevHp + ActualEnergy);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnEnergyZero.Broadcast();
	}
}

void UMyCharacterStatComponent::UpdatePower(float InEnergy)
{
	BaseStat.CharacterPower = BaseStat.CharacterPower + InEnergy;
	OnBatteryChanged.Broadcast(BaseStat);
}

void UMyCharacterStatComponent::InitEnergy(float InEnergy)
{
	BaseStat.InitialPower = InEnergy;
}

void UMyCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0, BaseStat.MaxHp);

	OnEnergyChanged.Broadcast(CurrentHp);
}
