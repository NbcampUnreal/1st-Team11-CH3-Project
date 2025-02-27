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
