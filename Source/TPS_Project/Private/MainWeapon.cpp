#include "MainWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

AMainWeapon::AMainWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
}

void AMainWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (WeaponMesh && WeaponMaterial)
    {
        WeaponMesh->SetMaterial(0, WeaponMaterial);
    }
}

void AMainWeapon::StartFiring()
{
    Fire();
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AMainWeapon::Fire, FireRate, true);
}

void AMainWeapon::StopFiring()
{
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AMainWeapon::Fire()
{
    LineTraceShoot();

    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMesh, TEXT("MuzzleSocket"));
    }
    /*
    FVector Start = WeaponMesh->GetComponentLocation();
    FVector ForwardVector = WeaponMesh->GetForwardVector();
    FVector End = Start + (ForwardVector * 5000.0f);

    if (bDebugLine)
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);

    }
    */
}

void AMainWeapon::LineTraceShoot()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    FVector Location;
    FRotator Rotation;
    PlayerController->GetPlayerViewPoint(Location, Rotation);
    FVector End = Location + (Rotation.Vector() * Range);

    FHitResult Hit;
    FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, TraceParams);

    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyPointDamage(HitActor, Damage, Rotation.Vector(), Hit, nullptr, this, nullptr);
        }

        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);
        }
    }

}


