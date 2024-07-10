// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/MyCharacterWidgetInterface.h"
#include "Interface/MyCharcaterItemInterface.h"
#include "Interface/MyCharacterHUDInterface.h"
#include "MyCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyCharacter2, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UMyItemDataAsset* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDeletgate) : ItemDeletgate(InItemDeletgate) {}
	
	FOnTakeItemDelegate ItemDeletgate;
};

UCLASS()
class PORT_API AMyCharacterBase : public ACharacter, public IMyCharacterWidgetInterface, public IMyCharcaterItemInterface, public IMyCharacterHUDInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UMyPrimaryDataAsset* ControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UMyPrimaryDataAsset*> CharacterControlManager;

	// Roll Action
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RollActionMontage;

	void ProcessRollCommand();

	// battery section
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Power)
	void PowerChangeEffect(float DivinePower);

	// Stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyCharacterStatComponent> Stat;

	// UI Widget
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UMyUserWidget* InUserWidget) override;
	virtual void SetupHUDWIdget(class UMyHUDUserWidget* HUDWidget) override;

protected:
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UMyItemDataAsset* InItemData) override;
	virtual void DrinkPotion(class UMyItemDataAsset* InItemData); // Score
	virtual void ReadScroll(class UMyItemDataAsset* InItemData); // Score

public:
	void UpdatePower(float InPower);

	UFUNCTION(BlueprintPure, Category = Power)
	float GetInitialPower();

	UFUNCTION(BlueprintPure, Category = Power)
	float GetCharacterPower();

};
