#pragma once

#include "CoreMinimal.h"
#include "OverlapBaseItem.h"
#include "AmmoBox.generated.h"

UCLASS()
class TPS_PROJECT_API AAmmoBox : public AOverlapBaseItem
{
	GENERATED_BODY()
	
public:
    AAmmoBox();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 Ammo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
    float RotationSpeed;

protected:
    virtual void Tick(float DeltaTime) override;
    virtual void ActivateItem(AActor* Activator) override;
};
