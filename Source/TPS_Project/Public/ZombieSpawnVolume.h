#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnRow.h"
#include "ZombieSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class TPS_PROJECT_API AZombieSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:
	AZombieSpawnVolume();

public:
	bool bIsSpawn;			// 제한 시간 버티면 스폰 안되도록 하기 위해
	float SpawnInterval;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn|Comp")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn|Comp")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|DataTable")
	UDataTable* ZombieDataTable;

	float TotalDeltaTime;

public:
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnRandomZombie();

protected:
	virtual void Tick(float DeltaTime) override;

	FVector GetRandomPointToSpawn() const;
	FZombieSpawnRow* GetRandomZombie() const;
	void SpawnZombie(TSubclassOf<AActor> ZombieClass);
};
