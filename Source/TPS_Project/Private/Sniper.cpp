#include "Sniper.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

ASniper::ASniper()
{
    FireRate = 1.5f;
    Damage = 90.0f;
    Range = 5000.0f;
}

void ASniper::StartFiring()
{
    Fire();
    ZoomOut();
	
}

void ASniper::ZoomIn()
{
    if (!PlayerController) PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    bIsZoomed = true;
    PlayerController->PlayerCameraManager->SetFOV(30.0f);
}

void ASniper::ZoomOut()
{
    if (!PlayerController) return;

    bIsZoomed = false;

    PlayerController->PlayerCameraManager->SetFOV(90.0f);
}

