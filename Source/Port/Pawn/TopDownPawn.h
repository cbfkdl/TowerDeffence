// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Interface/MyTowerHUDInterface.h"
#include "TopDownPawn.generated.h"

UCLASS()
class PORT_API ATopDownPawn : public APawn ,public IMyTowerHUDInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Scene;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;*/

	class UMyPrimaryDataAsset* CharacterControl;

	/** Build Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BuildAction;

	/** Composite Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CompositeAction;

	/** Destroy Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DestroyAction;

	/* 좌클 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameHp, meta = (AllowPrivateAccess = "true"))
	int MaxHp;

public:
	// Sets default values for this pawn's properties
	ATopDownPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Build(const FInputActionValue& Value);
	void CompositeActor(const FInputActionValue& Value);
	void BuildRnadomActor(const FInputActionValue& Value);
	void BuildRnadomActorReleased(const FInputActionValue& Value);
	void Destroy(const FInputActionValue& Value);

private:
	bool IsBuildMode;
	bool IsCompositedMode;

	// overlap에서 벗어난 actor의 색을 돌리기 위해서
	TObjectPtr<AActor> PrevActor;
	TArray<class AMyColorActor*> ColorActors;
	TArray<class ADragonCharacter*> AllTowers;
	TSubclassOf <class ADragonCharacter> WhatToSpawn;

	void ChangeColorCube(AActor* SelectedCube);


protected:
	virtual void SetupHUDWIdget(class UTowerHUDWidget* HUDWidget) override;

};
