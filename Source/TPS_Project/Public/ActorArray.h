#pragma once

#include "CoreMinimal.h"
#include "ActorArray.generated.h"

USTRUCT(BlueprintType)
struct FActorArray
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
    TArray<TSoftObjectPtr<AActor>> SpawnVolumes;
};
