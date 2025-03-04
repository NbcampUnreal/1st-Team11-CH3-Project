#include "StageStartTrigger.h"
#include "MainGameState.h"

AStageStartTrigger::AStageStartTrigger()
{
    bIsStageStarted = false;
}

void AStageStartTrigger::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (Activator && Activator->ActorHasTag("Player")) {
        if (!bIsStageStarted)
        {
            if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>())
            {
                MainGameState->StartGame();
                bIsStageStarted = true;
            }

            DestroyItem();
        }
    }
}
