#pragma once

#include "CoreMinimal.h"
#include "MainWeapon.h"
#include "Sniper.generated.h"

UCLASS()
class TPS_PROJECT_API ASniper : public AMainWeapon
{
	GENERATED_BODY()
	
public:	
	ASniper();
    //virtual void StartFire() override;

protected:
    virtual void BeginPlay() override;

private:
    void SetFOV(float NuwFOV);

    UPROPERTY(EditDefaultsOnly, Category="Sniper Settings")
    float ZoomedFOV = 30.0f;

    UPROPERTY(EditDefaultsOnly, Category="Sniper Settings")
    float DefaultFOV = 90.0f;
};
