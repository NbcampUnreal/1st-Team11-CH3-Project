#pragma once

#include "CoreMinimal.h"
#include "ActorArray.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"

UCLASS()
class TPS_PROJECT_API AMainGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMainGameState();

protected:
    FString CurrentLevel;
	int32 WaveCount;
	int32 MaxWaveCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Wave")
	float WaveInterval;			// Wave 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Level")
	float DefenceTime;			// 버텨야할 시간

    // Array도 포인터가 아니라 참조? 스마트 포인터 쪽으로도
    // 여기서 포인터로 받았다고 가정했을 때
    // 만약 해당 Actor가 Destroy()됬을 때 nullptr로 바꿔지지 않음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Zombie Spawners")
	TArray<AActor*> DefenceLevelSpawnVolumes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Zombie Spawners")
	TArray<AActor*> BossLevelSpawnVolumes;
    // Array의 참조나 포인터를 사용하는 식으로 생각해보기, 스마트 포인터 쪽으로도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Zombie Spawners")
    TMap<FString, FActorArray> SpawnVolumesByLevel;

	FTimerHandle LevelTimerHandle;
	FTimerHandle WaveStartTimerHandle;
	FTimerHandle WaveEndTimerHandle;

public:
	virtual void BeginPlay() override;

	void StartGame();
	void StartWave();
	void EndWave();
	void DefenceLevelTimeUp();
	void GameOver();
    void SetCurrentLevel(FString LevelName);
};
