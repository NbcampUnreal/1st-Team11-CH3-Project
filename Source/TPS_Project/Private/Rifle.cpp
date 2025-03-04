#include "Rifle.h"
#include "TimerManager.h"

ARifle::ARifle()
{
    FireRate = 0.08f;
}

void ARifle::BeginPlay()
{
    Super::BeginPlay();
}

void ARifle::Fire()
{
    if (AmmoCount > 0)
    {
        Super::Fire();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("라이플 탄약없음!"));
    }
}

void ARifle::StartFire()
{
    if (AmmoCount > 0)
    {
        GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ARifle::Fire, FireRate, true);
    }
}

void ARifle::StopFire()
{
    GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

void ARifle::Reload()
{
    Super::Reload();
}