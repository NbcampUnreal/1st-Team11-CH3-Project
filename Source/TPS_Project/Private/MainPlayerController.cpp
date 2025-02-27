// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

AMainPlayerController::AMainPlayerController() : InputMappingContext(nullptr), MoveAction(nullptr), JumpAction(nullptr), LookAction(nullptr), GunFireAction(nullptr)
,MainMenuWidgetClass(nullptr), MainMenuWidgetInstance(nullptr)
AMainPlayerController::AMainPlayerController() : InputMappingContext(nullptr), MoveAction(nullptr), JumpAction(nullptr), LookAction(nullptr), GunFireAction(nullptr), ReloadAction(nullptr)
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
}

//
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

void AMainPlayerController::StartGame()
{
	// 게임인스턴스 있으면 데이터 리셋

	UGameplayStatics::OpenLevel(GetWorld(), FName("DefenceLevel"));
	
	
	// todo; 임시방편으로,.. 원래 HUD 안에서 해야함
	bShowMouseCursor=false;
	SetInputMode(FInputModeGameOnly());

}

