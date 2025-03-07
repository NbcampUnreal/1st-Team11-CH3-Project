#pragma once

#include "CoreMinimal.h"
#include "OverlapBaseItem.h"
#include "HealKit.generated.h"

UCLASS()
class TPS_PROJECT_API AHealKit : public AOverlapBaseItem
{
	GENERATED_BODY()

public:
	AHealKit();

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item")
	float Heal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* MedBagPatch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* Scissors;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void ActivateItem(AActor* Activator) override;
};
