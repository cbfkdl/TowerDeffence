// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "UI/MyHUDUserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyHUDUserWidget> HUDRef(TEXT("/Game/ThirdPerson/UI/BatteryHUD.BatteryHUD_C"));
	if (HUDRef.Class)
	{
		HUDWidgetClass = HUDRef.Class;
	}

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	MyHUDWidget = CreateWidget<UMyHUDUserWidget>(this, HUDWidgetClass);
	if (MyHUDWidget)
	{
		MyHUDWidget->AddToViewport();
	}
}
