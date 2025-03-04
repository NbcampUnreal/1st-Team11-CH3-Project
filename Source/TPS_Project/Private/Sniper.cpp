#include "Sniper.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

ASniper::ASniper()
{
    FireRate = 1.5f;
}

void ASniper::BeginPlay()
{
    Super::BeginPlay();
}

void ASniper::Fire()
{
    if (AmmoCount > 0)
    {
        Super::Fire();
        AmmoCount--;
        SetFOV(ZoomedFOV);
        UE_LOG(LogTemp, Warning, TEXT("탄약 : %d"), AmmoCount);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("재장전 필요"));
    }
}

void ASniper::SetFOV(float NewFOV)
{
    APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
    if (PlayerController && PlayerController->PlayerCameraManager)
    {
        PlayerController->PlayerCameraManager->SetFOV(NewFOV);
    }
}