#include "MainGameState.h"
#include "ZombieSpawnVolume.h"
#include "Kismet/GameplayStatics.h"

AMainGameState::AMainGameState()
{
	WaveCount = 0;
	MaxWaveCount = 5;
	WaveInterval = 10.0f;
	DefenceTime = 60.0f;
    CurrentLevel = "";
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

    CurrentLevel = "DefenceLevel";

	StartGame();
}

void AMainGameState::StartGame()
{
    WaveCount = 0;

    if (CurrentLevel == "DefenceLevel")
    {
        // Defence Level의 Defence 타이머
        GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMainGameState::DefenceLevelTimeUp, DefenceTime, false);
    }

    // Wave 타이머
    GetWorldTimerManager().SetTimer(WaveStartTimerHandle, this, &AMainGameState::StartWave, WaveInterval, true, 10.0f);
}

// 스폰 주기 감소 -> 마리 수 증가
void AMainGameState::StartWave()
{
    if (WaveCount >= MaxWaveCount)
    {
        GetWorld()->GetTimerManager().ClearTimer(WaveStartTimerHandle);
        return;
    }

    if (SpawnVolumesByLevel[CurrentLevel].Actors.Num() > 0)
	{
        for (TSoftObjectPtr<AActor> SoftSpawnVolume : SpawnVolumesByLevel[CurrentLevel].Actors)
		{
            // LoadSynchronous()는 액터를 로드하는 것
            //AActor* SpawnVolume = SoftSpawnVolume.LoadSynchronous();
            AActor* SpawnVolume = SoftSpawnVolume.Get();
			if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
			{
                ZombieSpawnVolume->SpawnInterval = 1.0f;
			}
		}
	}

	WaveCount++;

	GetWorldTimerManager().SetTimer(WaveEndTimerHandle, this, &AMainGameState::EndWave, FMath::CeilToFloat(WaveInterval / 3.0f), false);
}

// 스폰 주기 정상화 -> 마리 수 감소
void AMainGameState::EndWave()
{
	if (WaveCount >= MaxWaveCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaveEndTimerHandle);
		return;
	}

    if (SpawnVolumesByLevel[CurrentLevel].Actors.Num() > 0)
    {
        for (TSoftObjectPtr<AActor> SoftSpawnVolume : SpawnVolumesByLevel[CurrentLevel].Actors)
        {
            AActor* SpawnVolume = SoftSpawnVolume.Get();
            if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
            {
                ZombieSpawnVolume->SpawnInterval = 2.0f;
            }
        }
    }
}

void AMainGameState::DefenceLevelTimeUp()
{
	if (DefenceLevelSpawnVolumes.Num() > 0)
	{
		for (AActor* SpawnVolume : DefenceLevelSpawnVolumes)
		{
			if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
			{
				// 제한 시간 끝나면 스폰 안되도록
                ZombieSpawnVolume->bIsSpawn = false;
			}
		}
	}
}

void AMainGameState::GameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Game Over!")));
}

void AMainGameState::SetCurrentLevel(FString LevelName)
{
    CurrentLevel = LevelName;
}
