#pragma once

#include "CoreMinimal.h"
#include "OverlapBaseItem.h"
#include "StageStartTrigger.generated.h"

UCLASS()
class TPS_PROJECT_API AStageStartTrigger : public AOverlapBaseItem
{
	GENERATED_BODY()
	
public:
    AStageStartTrigger();

    bool bIsStageStarted;

protected:
    virtual void ActivateItem(AActor* Activator) override;
};
