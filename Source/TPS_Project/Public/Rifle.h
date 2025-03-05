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
    //virtual void StartFire() override;

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle FireTimer;
};