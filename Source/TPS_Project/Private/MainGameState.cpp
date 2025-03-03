#include "MainGameState.h"
#include "ZombieSpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Animation/WidgetAnimation.h"


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
	// Wave 타이머
	GetWorldTimerManager().SetTimer(WaveStartTimerHandle, this, &AMainGameState::StartWave, WaveInterval, true, 10.0f);

	// Defence 타이머
	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMainGameState::LevelTimeUp, DefenceTime, false);

	// TODO; HUD 보이게 하는 코드..
	//...

    // 미션 UI 
    APlayerController *PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartGame: PlayerController를 찾을 수 없습니다!"));
        return;
    }
    // MainPlayerController로 캐스팅
    AMainPlayerController *MainPC = Cast<AMainPlayerController>(PC);
    if (!MainPC)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartGame: MainPlayerController 캐스팅 실패!"));
        return;
    }
    // 위젯 클래스를 확인
    if (!MainPC->MissionWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartGame: MissionWidgetClass가 설정되지 않았습니다!"));
        return;
    }
    // Mission UI 생성
    UUserWidget *MissionWidget = CreateWidget<UUserWidget>(MainPC, MainPC->MissionWidgetClass);
    if (!MissionWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartGame: MissionWidget 생성 실패!"));
        return;
    }
    // 위젯을 화면에 추가
    MissionWidget->AddToViewport();
    UE_LOG(LogTemp, Log, TEXT("StartGame: Mission UI 생성 완료!"));


    // 애니메이션 효과 추가
    UFunction *PlayAnimFunc = MissionWidget->FindFunction(FName("PlayMissionAnim"));
    if (PlayAnimFunc)
    {
        MissionWidget->ProcessEvent(PlayAnimFunc, nullptr);

    }

    WaveCount = 0;

    if (CurrentLevel == "DefenceLevel")
    {
        // Defence Level의 Defence 타이머
        GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMainGameState::DefenceLevelTimeUp, DefenceTime, false);
    }

    // Wave 타이머
    GetWorldTimerManager().SetTimer(WaveStartTimerHandle, this, &AMainGameState::StartWave, WaveInterval, true, 5.0f);
}

// 스폰 주기 감소 -> 마리 수 증가
void AMainGameState::StartWave()
{
    if (WaveCount >= MaxWaveCount)
    {
        GetWorld()->GetTimerManager().ClearTimer(WaveStartTimerHandle);
        return;
    }

    // Find(key)는 TMap에 Key가 들어있으면 value의 포인터를 반환하고, 없으면 nullptr 반환
    if (FActorArray* SpawnVolumes = SpawnVolumesByLevel.Find(CurrentLevel))
    {
        for (TSoftObjectPtr<AActor> SoftSpawnVolume : SpawnVolumes->SpawnVolumes)
        {
            if (AActor* SpawnVolume = SoftSpawnVolume.Get())
            {
                if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
                {
                    ZombieSpawnVolume->SpawnInterval = 1.0f;
                }
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
	}

    if (FActorArray* SpawnVolumes = SpawnVolumesByLevel.Find(CurrentLevel))
    {
        for (TSoftObjectPtr<AActor> SoftSpawnVolume : SpawnVolumes->SpawnVolumes)
        {
            if (AActor* SpawnVolume = SoftSpawnVolume.Get())
            {
                if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
                {
                    ZombieSpawnVolume->SpawnInterval = 2.0f;
                }
            }
        }
    }
}

void AMainGameState::DefenceLevelTimeUp()
{
    if (FActorArray* SpawnVolumes = SpawnVolumesByLevel.Find("DefenceLevel"))
    {
        for (TSoftObjectPtr<AActor> SoftSpawnVolume : SpawnVolumes->SpawnVolumes)
        {
            if (AActor* SpawnVolume = SoftSpawnVolume.Get())
            {
                if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
                {
                    ZombieSpawnVolume->bIsSpawn = false;
                }
            }
        }
    }
}

void AMainGameState::GameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Game Over!")));

	// todo; 게임오버시 메인메뉴보이게? ...

}


void AMainGameState::SetCurrentLevel(FName LevelName)
{
    CurrentLevel = LevelName;
}

FName AMainGameState::GetCurrentLevel() const
{
    return CurrentLevel;
}
