// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"
#include "MainGameState.h"

AMainPlayerController::AMainPlayerController() : 
    InputMappingContext(nullptr), 
    MoveAction(nullptr), 
    JumpAction(nullptr), 
    LookAction(nullptr), 
    MainMenuWidgetClass(nullptr), 
    MainMenuWidgetInstance(nullptr), 
    HUDWidgetClass(nullptr), 
    HUDWidgetInstance(nullptr), 
    GunFireAction(nullptr), 
    ReloadAction(nullptr)
{

}



void AMainPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Controller BeginPlay"));
	UEnhancedInputLocalPlayerSubsystem* SubSystem=GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	SubSystem->AddMappingContext(InputMappingContext, 0);

	// 게임 실행 시 메뉴 레벨에서 메뉴 UI 먼저 표시되도록 
	FString CurrentMapName=GetWorld()->GetMapName();
	if ( CurrentMapName.Contains("MenuLevel") )
	{
		ShowMainMenu(false); // false 는 처음 시작 나타냄 
	}

    // 디펜스레벨로 바뀌었다면..
    FString CurrentLevelName = GetWorld()->GetMapName();
    if (CurrentLevelName.Contains("DefenceLevel")) // 실제 언리얼에디터에서 설정한 레벨이름과 확인후..
    {

        ShowGameHud();

        // 미션 UI 생성 로직
        // 미션 UI 
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (!PC)
        {
            UE_LOG(LogTemp, Warning, TEXT("StartGame: PlayerController를 찾을 수 없습니다!"));
            return;
        }
        // MainPlayerController로 캐스팅
        AMainPlayerController* MainPC = Cast<AMainPlayerController>(PC);
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
        UUserWidget* MissionWidget = CreateWidget<UUserWidget>(MainPC, MainPC->MissionWidgetClass);
        if (!MissionWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("StartGame: MissionWidget 생성 실패!"));
            return;
        }
        // 위젯을 화면에 추가
        MissionWidget->AddToViewport();
        UE_LOG(LogTemp, Log, TEXT("StartGame: Mission UI 생성 완료!"));


        // 애니메이션 효과 추가
        UFunction* PlayAnimFunc = MissionWidget->FindFunction(FName("PlayMissionAnim"));
        if (PlayAnimFunc)
        {
            MissionWidget->ProcessEvent(PlayAnimFunc, nullptr);
        }
    }
}

void AMainPlayerController::ShowMainMenu(bool bIsRestart)
{
	// HUD 있다면 닫기

	// 이미 메인 메뉴가 떠 있다면 제거하기
	if ( MainMenuWidgetInstance )
	{
		MainMenuWidgetInstance->RemoveFromParent(); //
		MainMenuWidgetInstance=nullptr;
	}

	// 메인 메뉴 생성
	if (MainMenuWidgetClass)
	{
		// 메인 메뉴 만든후 뷰포트 설정 및 마우스와 UI만 사용가능하게 
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if ( MainMenuWidgetInstance ) // 만들어졌다면
		{
			// 뷰포트에 보이기 설정
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor=true; //마우스 커서 보이게
			SetInputMode(FInputModeUIOnly());  // UI 만 되게 설정
		}

		// 버튼작업
		if (UTextBlock* ButtonText= Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if ( bIsRestart )
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart"))); // FSTRING->FText로 바꾼후 바인딩
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}
	}
}

void AMainPlayerController::ShowGameHud()
{
    // 이미 존재했다면 지우고 시작
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    // 이미 메뉴가 떠 있다면 혹시 모르니 지우기
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());

            // GetWorld() 가 존재한다면... 게임스테이트에서 얻어오기 
            AMainGameState* MainGameState =
                GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
            if (MainGameState)
            {
                MainGameState->UpdateHUD();
            }
        }
    }
}

UUserWidget* AMainPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

void AMainPlayerController::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("DefenceLevel"));
	
	bShowMouseCursor=false;
	SetInputMode(FInputModeGameOnly());
}

void AMainPlayerController::StartMainMenu()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}

void AMainPlayerController::ShowGameOverMenu(bool bIsClear)
{
    // 메뉴를 보여주려면 현재 보여지고 있는 위젯을 제거해야함
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (GameOverMenuWidgetInstance)
    {
        GameOverMenuWidgetInstance->RemoveFromParent();
        GameOverMenuWidgetInstance = nullptr;
    }

    if (GameOverMenuWidgetClass)
    {
        GameOverMenuWidgetInstance = CreateWidget<UUserWidget>(this, GameOverMenuWidgetClass);

        if (GameOverMenuWidgetInstance)
        {
            GameOverMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());

            if (bIsClear)
            {
                if (UTextBlock* GameOverText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName(TEXT("GameOverText"))))
                {
                    GameOverText->SetText(FText::FromString(TEXT("Clear")));
                }
            }
            else
            {
                if (UTextBlock* GameOverText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName(TEXT("GameOverText"))))
                {
                    GameOverText->SetText(FText::FromString(TEXT("Game Over")));
                }
            }
        }
    }
}

void AMainPlayerController::ExitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}

