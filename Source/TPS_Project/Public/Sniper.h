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

    virtual void StartFiring() override;
    void ZoomIn();
    void ZoomOut();

private:
    bool bIsZoomed = false;
    APlayerController* PlayerController;
};
