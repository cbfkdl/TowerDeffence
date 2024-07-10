// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyTopDownController.h"
#include "UI/TowerHUDWidget.h"

AMyTopDownController::AMyTopDownController()
{
	bShowMouseCursor = true;
	static ConstructorHelpers::FClassFinder<UTowerHUDWidget> HUDRef(TEXT("/Game/ThirdPerson/UI/TowerHUD.TowerHUD_C"));
	if (HUDRef.Class)
	{
		HUDWidgetClass = HUDRef.Class;
	}
}

void AMyTopDownController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI GameAndUI;
	SetInputMode(GameAndUI);

	MyHUDWidget = CreateWidget<UTowerHUDWidget>(this, HUDWidgetClass);
	if (MyHUDWidget)
	{
		MyHUDWidget->AddToViewport();
	}
}
