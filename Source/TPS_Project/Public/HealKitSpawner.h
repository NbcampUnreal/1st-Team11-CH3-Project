#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealKitSpawner.generated.h"

class AHealKit;

UCLASS()
class TPS_PROJECT_API AHealKitSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AHealKitSpawner();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner|Component")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Class")
	TSubclassOf<AHealKit> HealKitClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Properties")
	float SpawnInterval;
	bool bIsTimerSet;
	AHealKit* SpawnedHealKit;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnHealKit();
};
