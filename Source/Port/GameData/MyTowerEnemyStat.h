
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyTowerEnemyStat.generated.h"

USTRUCT(BlueprintType)
struct FMyTowerEnemyStat : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FMyTowerEnemyStat() : MaxHp(0.0f), MovementSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

};