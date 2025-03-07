#pragma once

#include "CoreMinimal.h"
#include "ZombieSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FZombieSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ZombieType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ZombieClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
