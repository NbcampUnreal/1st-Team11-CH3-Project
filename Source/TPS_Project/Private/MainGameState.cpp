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
	WaveInterval = 10.0f;
	DefenceTime = 60.0f;
}

void AMainGameState::BeginPlay()
{
    //TODO; 뭔가 있겠지..
	Super::BeginPlay();

    FString CurrentLevelName = GetWorld()->GetMapName();
    if (CurrentLevelName.Contains("DefenceLevel"))
    {
        CurrentLevel = "DefenceLevel";
        StartGame();

        // 레벨이 완전히 로드되기 전에 라이트 관련 업데이트를 하면 업데이트 적용이 안됨
        // => 약간 텀을 두고 업데이트
        //GetWorld()->GetTimerManager().SetTimer(LightUpdateTimerHandle, this, &AMainGameState::UpdateLightSettings, 1.0f, false);
        // SetTimerForNextTick : 새로운 프레임이 시작될 때 실행
        //GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMainGameState::UpdateLightSettings);

        // Delegate에 바인딩되는 함수는 void ::함수이름(UWorld* LoadedWorld) 형태의 시그니처를 가져야함
        //FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &AMainGameState::UpdateLightSettingsDelegate);

        // Delegate에 바인딩되는 함수는 void ::함수이름(UWorld* World, ELevelTick TickType, float DeltaTime) 형태의 시그니처를 가져야함
        // OnWorldPostActorTick : 현재 프레임 끝나기 직전에 실행
        //FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &AMainGameState::UpdateLightFirstFrame);
    }
}

// SkyLight 업데이트
void AMainGameState::UpdateSkyLight()
{
    TArray<AActor*> SkyLights;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASkyLight::StaticClass(), SkyLights);

    for (AActor* Actor : SkyLights)
    {
        ASkyLight* SkyLight = Cast<ASkyLight>(Actor);
        if (SkyLight)
        {
            USkyLightComponent* SkyLightComp = SkyLight->GetLightComponent();
            if (SkyLightComp)
            {
                // SkyLight 강제 업데이트
                SkyLightComp->RecaptureSky();

                // Sky Light의 위치가 변경되면 자동으로 환경을 다시 캡처
                //FVector NewLocation = SkyLight->GetActorLocation() + FVector(0, 0, 1); // 1cm 이동
                //SkyLight->SetActorLocation(NewLocation);

                // Sky Light를 숨겼다가 다시 보이게 하면 자동으로 환경을 캡처
                //SkyLight->SetActorHiddenInGame(true);
                //FTimerHandle TimerHandle;
                //GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SkyLight]()
                //    {
                //        SkyLight->SetActorHiddenInGame(false);
                //    }, 0.1f, false);
            }
        }
    }
}

// 렌더링 관련 콘솔 명령어 실행
void AMainGameState::ExecuteConsoleCommands()
{
    if (GEngine)
    {
        GEngine->Exec(GetWorld(), TEXT("r.Cache.LightingCache 0"));
        
        GEngine->Exec(GetWorld(), TEXT("r.EyeAdaptationQuality 0"));
        GEngine->Exec(GetWorld(), TEXT("r.OneFrameThreadLag 0"));

        //GEngine->Exec(GetWorld(), TEXT("r.ForceAllCastsDynamicShadow 1"));  // 그림자 동적 적용
        //GEngine->Exec(GetWorld(), TEXT("r.TonemapperFilm 1"));              // 색상 톤 매핑을 강제 적용하여 밝기 보정
        //GEngine->Exec(GetWorld(), TEXT("r.ExposureOffset 1"));              // 전체 노출 값 조정
        //GEngine->Exec(GetWorld(), TEXT("r.LightPropagationVolume 1"));      // 라이팅을 강제로 다시 적용
        //GEngine->Exec(GetWorld(), TEXT("r.HZBOcclusion 0"));                // 레벨 로드 후 씬 가시성 오류 해결
        //
        //GEngine->Exec(GetWorld(), TEXT("r.ClearScene 1"));                  // 씬을 강제로 다시 렌더링
        //GEngine->Exec(GetWorld(), TEXT("r.TemporalAA.Upsampling 1"));       // TAA(Temporal Anti-Aliasing) 문제 해결
    }
}

// Post Process Volume의 Auto Expsure 비활성화
void AMainGameState::DisableAutoExposure()
{
    TArray<AActor*> PostProcessVolumes;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), PostProcessVolumes);

    UE_LOG(LogTemp, Warning, TEXT("Disable Auto Exposure Check"));

    for (AActor* Actor : PostProcessVolumes)
    {
        if (APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(Actor))
        {
            PostProcessVolume->bUnbound = true;
            // Auto Exposure 값 강제 고정
            PostProcessVolume->Settings.AutoExposureMinBrightness = 1.0f;
            PostProcessVolume->Settings.AutoExposureMaxBrightness = 1.0f;
            // Auto Exposure 값 강제 적용
            PostProcessVolume->Settings.bOverride_AutoExposureMinBrightness = true;
            PostProcessVolume->Settings.bOverride_AutoExposureMaxBrightness = true;
            
            UE_LOG(LogTemp, Warning, TEXT("Disable Auto Exposure ON"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Disable Auto Exposure NO"));
        }
    }
}

void AMainGameState::UpdateLightSettings()
{
    UE_LOG(LogTemp, Warning, TEXT("Update Light Settings"));

    UpdateSkyLight();

    //ExecuteConsoleCommands();
}

void AMainGameState::UpdateLightSettingsDelegate(UWorld* LoadedWorld)
{
    UE_LOG(LogTemp, Warning, TEXT("Update Light Settings Delegate"));

    UpdateSkyLight();

    //ExecuteConsoleCommands();
}

void AMainGameState::UpdateLightFirstFrame(UWorld* World, ELevelTick TickType, float DeltaTime)
{
    FrameCount++;

    //DisableAutoExposure();

    if (World == GetWorld())
    {
        UpdateLightSettings();
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMainGameState::UpdateSkyLight);

        UE_LOG(LogTemp, Warning, TEXT("FrameCount = %d"), FrameCount);

        if (FrameCount >= 1) {
            // 첫 프레임에 한 번만 실행되도록
            FWorldDelegates::OnWorldPostActorTick.RemoveAll(this);
        }
    }
}

void AMainGameState::StartGame()
{
	//// Wave 타이머
	//GetWorldTimerManager().SetTimer(WaveStartTimerHandle, this, &AMainGameState::StartWave, WaveInterval, true, 10.0f);

	//// Defence 타이머
	//GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AMainGameState::LevelTimeUp, DefenceTime, false);

	// TODO; HUD 보이게 하는 코드..-> 실제 DefenceLevel일때 
    // FString CurrentLevelName = GetWorld()->GetMapName();
    //if (CurrentLevelName.Contains("DefenceLevel")) // 실제 언리얼에디터에서 설정한 레벨이름과 확인후..
	//...



   
    WaveCount = 0;

    // startWave로 안가는데?;';

    // ;타이머 HUD와 연동..
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

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Start Game : %s"), *CurrentLevel.ToString()));

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
    GetWorldTimerManager().SetTimer(WaveStartTimerHandle, this, &AMainGameState::StartWave, WaveInterval, true, 5.0f);
}

// 스폰 주기 감소 -> 마리 수 증가
void AMainGameState::StartWave()
{
    //FString CurrentLevelName = GetWorld()->GetMapName();
    //if (CurrentLevelName.Contains("DefenceLevel")) // 실제 언리얼에디터에서 설정한 레벨이름과 확인후..
    //{
    //    // Wave 타이머 -> UpdateHUD로 변경; 매 시간? 마다 UpdateHUD해주기 
    //    GetWorldTimerManager().SetTimer(
    //        HUDUpdateTimerHandle,
    //        this,
    //        &AMainGameState::UpdateHUD,
    //        0.1f,//0.1초마다
    //        true
    //        );
    //}



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
}

void AMainGameState::GameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Game Over!")));

	// todo; 게임오버시 메인메뉴보이게? ...

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

            // TODO; 다양한 HUD 업데이트 코드 
            
            // 캐릭터 체력UI 업데이트 HealthBar
           if(UProgressBar* HPBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
           {
               // Health / MaxHealth 
               float HPPercent = 0.f;
               if (100.f/*MaxHealth*/ > 0.f)
               {
                   ACharacter * PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
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
            
            // UpdateHealthBar(); 

            // 점수?

            // Wave 
            if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
            {
                WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), WaveCount+1/*0으로시작해서..*/)));
            }


        }

    }
    


}

FName AMainGameState::GetCurrentLevel() const
{
    return CurrentLevel;
}
