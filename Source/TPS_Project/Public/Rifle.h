#pragma once

#include "CoreMinimal.h"
#include "MainWeapon.h"
#include "Rifle.generated.h"

UCLASS()
class TPS_PROJECT_API ARifle : public AMainWeapon
{
    GENERATED_BODY()

public:
    ARifle();
    virtual void Fire() override;

protected:
    virtual void BeginPlay() override;

private:
    void StartFire();
    void StopFire();

    FTimerHandle FireTimer;
};