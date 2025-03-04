#include "AmmoBox.h"
#include "MainCharacter.h"

AAmmoBox::AAmmoBox()
{
    PrimaryActorTick.bCanEverTick = true;

    ItemType = "AmmoBox";
    Ammo = 100;
    RotationSpeed = 90.0f;
}

void AAmmoBox::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}

void AAmmoBox::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (AMainCharacter* Player = Cast<AMainCharacter>(Activator))
        {
            // 플레이어 탄창 수 증가
        }

        DestroyItem();
    }
}
