// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyWidgetComponent.h"
#include "UI/MyUserWidget.h"

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMyUserWidget* MyUserWIdget = Cast<UMyUserWidget>(GetWidget());
	if (MyUserWIdget)
	{
		MyUserWIdget->SetOwningActor(GetOwner());
	}
}
