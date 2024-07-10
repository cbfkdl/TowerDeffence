// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/MyTowerCubeInterface.h"
#include "MyColorActor.generated.h"

UCLASS()
class PORT_API AMyColorActor : public AActor, public IMyTowerCubeInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyColorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Tower)
	float ColorIndex;

	// 터렛이 지어진 블럭인지
	bool IsOnTurret;

public:
	virtual void SetTurret(bool OnOff) override { IsOnTurret = OnOff; }
	virtual bool GetIsOnTurret() override { return IsOnTurret; }
	virtual void SetOnTower(ADragonCharacter* _Tower) override;
	virtual class ADragonCharacter* GetOnTower() override { return Tower; }
	virtual enum class EAttackType GetAttackType() override;

protected:
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Color)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	TObjectPtr<class ADragonCharacter> Tower;

public:
	virtual void ChangeColor(FVector Color = FVector(1.0f,1.0f,1.0f), float Alpha = 1.0f) override;


};
