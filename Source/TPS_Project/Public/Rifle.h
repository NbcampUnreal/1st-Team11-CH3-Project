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
    virtual void Reload() override;
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle FireTimer;
};