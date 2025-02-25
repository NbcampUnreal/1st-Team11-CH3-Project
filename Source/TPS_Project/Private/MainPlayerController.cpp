// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMainPlayerController::AMainPlayerController() : InputMappingContext(nullptr), MoveAction(nullptr), JumpAction(nullptr), LookAction(nullptr), GunFireAction(nullptr)
{
}

void AMainPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Controller BeginPlay"));
	UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	SubSystem->AddMappingContext(InputMappingContext, 0);

}
