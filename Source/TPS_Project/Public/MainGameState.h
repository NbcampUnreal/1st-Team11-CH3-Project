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
    int32 FrameCount = 0;
    FName CurrentLevel;
	int32 WaveCount;
	int32 MaxWaveCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Wave")
	float WaveInterval;			// Wave 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Level")
	float DefenceTime;			// 버텨야할 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState|Zombie Spawners")
    TMap<FName, FActorArray> SpawnVolumesByLevel;   // 각 레벨에 배치된 좀비 스폰 볼륨들

	FTimerHandle LevelTimerHandle;
	FTimerHandle WaveStartTimerHandle;
	FTimerHandle WaveEndTimerHandle;
    FTimerHandle HUDUpdateTimerHandle;
    FTimerHandle LightUpdateTimerHandle;


public:
	virtual void BeginPlay() override;

	void StartGame();
	void StartWave();
	void EndWave();
	void DefenceLevelTimeUp();
	void GameOver(bool bIsClear);

    // HUD 어떤식으로 정보 업데이트 할건지? (웨이브시간초, 체력바, 총알개수?) 
    void UpdateHUD();

    void SetCurrentLevel(FName Level);
    FName GetCurrentLevel() const;
};
