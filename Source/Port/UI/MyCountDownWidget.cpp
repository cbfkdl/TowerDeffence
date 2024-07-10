// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyCountDownWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/ColorGameModeBase.h"
#include "Fonts/SlateFontInfo.h"

UMyCountDownWidget::UMyCountDownWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMyCountDownWidget::UpdateCountDown(int Count)
{
	FName NameKey(FString::Printf(TEXT("Count")));

	FString FormatSpec = *FString::Printf(TEXT("%d"), Count);
	UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
	if (BaseTextBlock)
	{
		if (Count == 0)
		{
			BaseTextBlock->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (Count == -1)
		{
			FormatSpec = *FString::Printf(TEXT("You Win"));

			// ���� ��Ʈ ������ ��������
			FSlateFontInfo FontInfo = BaseTextBlock->GetFont();
			// ��Ʈ ũ�� ����
			FontInfo.Size = 200;
			// ��Ʈ ���� ����
			BaseTextBlock->SetFont(FontInfo);
			BaseTextBlock->SetText(FText::FromString(FormatSpec));

			BaseTextBlock->SetVisibility(ESlateVisibility::Visible);
			return;
		}
		else if (Count == -2)
		{
			// ���� ��Ʈ ������ ��������
			FSlateFontInfo FontInfo = BaseTextBlock->GetFont();
			// ��Ʈ ũ�� ����
			FontInfo.Size = 200;
			// ��Ʈ ���� ����
			BaseTextBlock->SetFont(FontInfo);

			BaseTextBlock->SetVisibility(ESlateVisibility::Visible);
			FormatSpec = *FString::Printf(TEXT("Game Over"));
			BaseTextBlock->SetText(FText::FromString(FormatSpec));
			return;
		}

		BaseTextBlock->SetText(FText::FromString(FormatSpec));
	}
}
