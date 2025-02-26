#include "MainGameState.h"
#include "ZombieSpawnVolume.h"
#include "Kismet/GameplayStatics.h"

AMainGameState::AMainGameState()
{
	WaveCount = 0;
	MaxWaveCount = 5;
	WaveInterval = 10.0f;
	DefenceTime = 60.0f;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	StartGame();
}

void AMainGameState::StartGame()
{
	// Wave 타이머
	GetWorldTimerManager().SetTimer(WaveStartTimerHandle, this, &AMainGameState::StartWave, WaveInterval, true, 10.0f);

	// Defence 타이머
	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMainGameState::LevelTimeUp, DefenceTime, false);

	// TODO; HUD 보이게 하는 코드..
	//...


}

// 스폰 주기 감소 -> 마리 수 증가
void AMainGameState::StartWave()
{
	if (WaveCount >= MaxWaveCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaveStartTimerHandle);
		return;
	}

	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainGameState::StaticClass(), SpawnVolumes);
	if (SpawnVolumes.Num() > 0)
	{
		for (AActor* SpawnVolume : SpawnVolumes)
		{
			AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume);
			if (ZombieSpawnVolume)
			{
				// 스폰 주기 감소
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

	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieSpawnVolume::StaticClass(), SpawnVolumes);
	if (SpawnVolumes.Num() > 0)
	{
		for (AActor* SpawnVolume : SpawnVolumes)
		{
			AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume);
			if (ZombieSpawnVolume)
			{
				// 스폰 주기 정상화
				ZombieSpawnVolume->SpawnInterval = 2.0f;
			}
		}
	}
}

void AMainGameState::LevelTimeUp()
{
	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieSpawnVolume::StaticClass(), SpawnVolumes);
	if (SpawnVolumes.Num() > 0)
	{
		for (AActor* SpawnVolume : SpawnVolumes)
		{
			AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume);
			if (ZombieSpawnVolume)
			{
				// 제한 시간 끝나면 스폰 안되도록
				ZombieSpawnVolume->bIsSpawn = false;
			}
		}
	}

	GameOver();
}

void AMainGameState::GameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Game Over!")));

	// todo; 게임오버시 메인메뉴보이게? ...

}