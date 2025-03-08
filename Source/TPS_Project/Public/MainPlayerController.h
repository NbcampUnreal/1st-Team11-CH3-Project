// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
//class UUserWidget;

UCLASS()
class TPS_PROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* GunFireAction;

public:
	// 메인 메뉴 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Menu");
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Menu")
	UUserWidget* MainMenuWidgetInstance;

    // UMG 위젯 클래스 에디터에서 할당받을 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UUserWidget* HUDWidgetInstance;


    // 미션 UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Mission")
    TSubclassOf<UUserWidget> MissionWidgetClass;

    // 게임 오버 메뉴 UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu");
    TSubclassOf<UUserWidget> GameOverMenuWidgetClass;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
    UUserWidget* GameOverMenuWidgetInstance;


    



public:
	// 메인메뉴 보여주기
	UFUNCTION(BlueprintCallable, Category="Menu")
	void ShowMainMenu(bool bIsRestart);

    // HUD표시
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowGameHud();

    // 현재 가지고있는 HUD 위젯 리턴
    UFUNCTION(BlueprintPure, Category = "HUD")
    UUserWidget* GetHUDWidget() const;


	UFUNCTION(BlueprintCallable, Category="Menu")
	// MainMenu-> Game 시작
	void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void StartMainMenu();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ShowGameOverMenu(bool bIsClear);

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ExitGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ReloadAction;




protected:
	virtual void BeginPlay() override;
};