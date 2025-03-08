#include "MainGameState.h"
#include "ZombieSpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/SkyLightComponent.h"
#include "Engine/SkyLight.h"
#include "MainCharacter.h"


AMainGameState::AMainGameState()
{
    CurrentLevel = "";
	WaveCount = 0;
	MaxWaveCount = 5;
	WaveInterval = 30.0f;
	DefenceTime = 180.0f;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
   
    FString CurrentLevelName = GetWorld()->GetMapName();
    if (CurrentLevelName.Contains("DefenceLevel"))
    {
        CurrentLevel = "DefenceLevel";
        StartGame();
    }

}

void AMainGameState::StartGame()
{
    WaveCount = 0;

    FString CurrentLevelName = GetWorld()->GetMapName();
    if (CurrentLevelName.Contains("DefenceLevel")) // 실제 언리얼에디터에서 설정한 레벨이름과 확인후..
    {
        // Wave 타이머 -> UpdateHUD로 변경; 매 시간? 마다 UpdateHUD해주기 
        GetWorldTimerManager().SetTimer(
            HUDUpdateTimerHandle,
            this,
            &AMainGameState::UpdateHUD,
            0.1f,//0.1초마다
            true
        );
    }

    if (FActorArray* SpawnVolumes = SpawnVolumesByLevel.Find(CurrentLevel))
    {
        for (TSoftObjectPtr<AActor> SoftSpawnVolume : SpawnVolumes->SpawnVolumes)
        {
            if (AActor* SpawnVolume = SoftSpawnVolume.Get())
            {
                if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
                {
                    ZombieSpawnVolume->bIsSpawn = true;
                }
            }
        }
    }

    // Defence 타이머
    GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMainGameState::DefenceLevelTimeUp, DefenceTime, false);
  
    // Wave 타이머
    GetWorldTimerManager().SetTimer(WaveStartTimerHandle, this, &AMainGameState::StartWave, WaveInterval, true, 20.0f);
}

// 스폰 주기 감소 -> 마리 수 증가
void AMainGameState::StartWave()
{
    if (WaveCount >= MaxWaveCount)
    {
        GetWorld()->GetTimerManager().ClearTimer(WaveStartTimerHandle);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Start Wave : %d"), WaveCount + 1);

    // Find(key)는 TMap에 Key가 들어있으면 value의 포인터를 반환하고, 없으면 nullptr 반환
    if (FActorArray* SpawnVolumes = SpawnVolumesByLevel.Find(CurrentLevel))
    {
        for (TSoftObjectPtr<AActor> SoftSpawnVolume : SpawnVolumes->SpawnVolumes)
        {
            if (AActor* SpawnVolume = SoftSpawnVolume.Get())
            {
                if (AZombieSpawnVolume* ZombieSpawnVolume = Cast<AZombieSpawnVolume>(SpawnVolume))
                {
                    ZombieSpawnVolume->SpawnInterval = 3.5f;
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
                    ZombieSpawnVolume->SpawnInterval = 7.0f;
                }
            }
        }
    }
}

void AMainGameState::DefenceLevelTimeUp()
{
    UE_LOG(LogTemp, Warning, TEXT("Time Up"));

    if (FActorArray* SpawnVolumes = SpawnVolumesByLevel.Find(CurrentLevel))
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
   
    GameOver(true);
}

void AMainGameState::GameOver(bool bIsClear)
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
        {
            MainPlayerController->SetPause(true);
            if (bIsClear)
            {
                MainPlayerController->ShowGameOverMenu(bIsClear);
            }
            else
            {
                MainPlayerController->ShowGameOverMenu(bIsClear);
            }
        }
    }
}

void AMainGameState::SetCurrentLevel(FName Level)
{
    CurrentLevel = Level;
}

void AMainGameState::UpdateHUD()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        // 다운캐스팅해서 컨트롤러 받아오기
        AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController);

        // 인스턴스 얻어오기 + 얻어왔다면..
        if (UUserWidget* HUDWidget = MainPlayerController->GetHUDWidget())
        {
            // 시간 UI 업데이트
            if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
            {
                // 게임스테이트에서 설정해둔 레벨 타이머 핸들, TODO; 일단은 레벨타이머만...;
                float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);

                // 만약 0보다 작아진다면 0이되도록해라
                RemainingTime = FMath::Clamp(RemainingTime, 0.0f, DefenceTime);

                // UI에 어떻게 보여질건지 FString형식에서 -> FText 형식으로 변환해서 UI에 설정
                TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
            }

            // 캐릭터 체력UI 업데이트 HealthBar
            if (UProgressBar* HPBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
            {
                // Health / MaxHealth 
                float HPPercent = 0.f;
                if (100.f > 0.f)
                {
                    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
                    AMainCharacter* MainPlayerCharacter = Cast<AMainCharacter>(PlayerCharacter);
                    HPPercent = MainPlayerCharacter->GetCharacterHealth() / 100.f;
                }
                HPBar->SetPercent(HPPercent);

                // HPPercent가 낮으면 빨갛게 
                if (HPPercent < 0.3f)
                {
                    FLinearColor LowHPColor = FLinearColor::Red;
                    HPBar->SetFillColorAndOpacity(LowHPColor);
                }
                else
                {
                    FLinearColor LowHPColor = FLinearColor::Green;
                    HPBar->SetFillColorAndOpacity(LowHPColor);
                }
            }

            // 총알 텍스트 UI
            if (UTextBlock* BulletCountText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("BulletCount"))))
            {
                ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
                AMainCharacter* MainPlayerCharacter = Cast<AMainCharacter>(PlayerCharacter);
                if (MainPlayerCharacter)
                {

                    BulletCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),
                        MainPlayerCharacter->CurrentAmmo()
                        , MainPlayerCharacter->GetMaxAmmo())));

                }
                else
                {
                    BulletCountText->SetText(FText::FromString(TEXT("Ammo: N/A"))); // 안전한 기본값
                }
            }
            // Wave 
            if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
            {
                WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), WaveCount + 1/*0으로시작해서..*/)));
            }
        }
    }
}

FName AMainGameState::GetCurrentLevel() const
{
    return CurrentLevel;
}
