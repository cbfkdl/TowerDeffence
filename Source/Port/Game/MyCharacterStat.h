#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FMyCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMyCharacterStat() : MaxHp(0.0f), CharacterPower(0.0f), InitialPower(0.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CharacterPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float InitialPower;

	// 속도에 곱해질 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float SpeedFactor;

	// 파워레벨 0 기준 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float BaseSpeed;

	FMyCharacterStat operator+(const FMyCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FMyCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FMyCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};