// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/MyAnimationAttackInterface.h"
#include "Interface/MyCharacterWidgetInterface.h"
#include "Interface/MyTowerAllInterface.h"
#include "DragonCharacter.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	ESingle = 0,
	EMulty,	
	ENone,
};

UCLASS()
class PORT_API ADragonCharacter : public ACharacter, public IMyAnimationAttackInterface, public IMyCharacterWidgetInterface, public IMyTowerAllInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADragonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ClawActionMontage;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	EAttackType AttackType;

	void ProcessAttack();
	void ComboActionBegin();
	void ComboACtionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyAttackEnd();

	FTimerHandle AttackTimer;

	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Dead 
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	TObjectPtr<class UParticleSystemComponent> Effect;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 1.0f;

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);

	// UI
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyTowerStatComponent> Stat;

	// UI widget
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UMyUserWidget* UserWidget) override;

public:
	int IsUpgradeable(const ADragonCharacter* Other);
	void SetTowerLevel(int _Level);
	void SetAttackType(bool SetLevel = false);
	void SetOwnedCube(AActor* Cube);
	FORCEINLINE EAttackType GetAttackType() const { return AttackType; }
	FORCEINLINE class AMyColorActor* GetOwnedCube() const { return OwnedCube; }
	FORCEINLINE int GetTowerLevel() const { return TowerLevel; }

	virtual float GetAIDetectRange() override;
	virtual void SetAIAttackDelegate(const FAITowerAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI(APawn* InTarget) override;

	FAITowerAttackFinished OnAttackFinished;

private:
	int TowerLevel;
	float BlueColorBias;
	TObjectPtr<APawn> Target;
	TObjectPtr<class AMyColorActor> OwnedCube;
	void ChangeColor(FVector Color = FVector(1.0f, 1.0f, 1.0f), float Alpha = 1.0f);

};
