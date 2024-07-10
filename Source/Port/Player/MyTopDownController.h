// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyTopDownController.generated.h"

/**
 * 
 */
UCLASS()
class PORT_API AMyTopDownController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyTopDownController();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UTowerHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UTowerHUDWidget> MyHUDWidget;
};
