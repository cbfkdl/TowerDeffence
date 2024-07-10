// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/MyCharacterStat.h"
#include "MyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnergyZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnergyChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBatteryChangedDelegate, const FMyCharacterStat& /* Stat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORT_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	
public:
	FOnEnergyZeroDelegate OnEnergyZero;
	FOnEnergyChangedDelegate OnEnergyChanged;
	FOnBatteryChangedDelegate OnBatteryChanged;

	FORCEINLINE void SetBaseStat(const FMyCharacterStat& InBaseStat) { BaseStat = InBaseStat; }
	FORCEINLINE void SetModifierStat(const FMyCharacterStat& InModifierStat) { ModifierStat = InModifierStat; }


	FORCEINLINE FMyCharacterStat GetBaseStat() const { return BaseStat; }
	FORCEINLINE FMyCharacterStat GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FMyCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetInitialPower() { return BaseStat.InitialPower; }
	FORCEINLINE float GetCharacterPower() { return BaseStat.CharacterPower; }

	float GetCalculateWalkSpeed();

	void UpdateHp(float InHp);
	void UpdatePower(float InEnergy);
	void InitEnergy(float InEnergy);

	
protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FMyCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FMyCharacterStat ModifierStat;

};
