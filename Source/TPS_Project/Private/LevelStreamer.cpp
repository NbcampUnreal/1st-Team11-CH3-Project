#include "LevelStreamer.h"
#include "MainCharacter.h"
#include "MainGameState.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALevelStreamer::ALevelStreamer()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	SetRootComponent(OverlapVolume);

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelStreamer::OverlapBegin);

    bIsLoaded = false;
}

void ALevelStreamer::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsLoaded)
    {
        if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
        {
            if (AMainCharacter* Player = Cast<AMainCharacter>(Character))
            {
                // 겹치는 Actor에 대해 검사하고, 레벨 이름도 검사
                if (OtherActor == Player && LevelToLoad != "")
                {
                    // 서브 레벨 로드
                    FLatentActionInfo LatentInfo;
                    UGameplayStatics::LoadStreamLevel(GetWorld(), LevelToLoad, true, true, LatentInfo);
                    
                    bIsLoaded = true;
                }
            }
        }

        if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>())
        {
            MainGameState->SetCurrentLevel(LevelToLoad);
        }
    }
}