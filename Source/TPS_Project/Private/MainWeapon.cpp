#include "MainWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AMainWeapon::AMainWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    // 스태틱 메시 생성
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
}

void AMainWeapon::BeginPlay()
{
    Super::BeginPlay();

    // 머터리얼 적용
    if (WeaponMesh && WeaponMaterial)
    {
        WeaponMesh->SetMaterial(0, WeaponMaterial);
    }
}

void AMainWeapon::Fire()
{
    if (AmmoCount > 0 && bCanFire)
    {
        PerformLineTrace();
        AmmoCount--;
        UE_LOG(LogTemp, Warning, TEXT("남은 탄약 : %d"), AmmoCount);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 없음! 재장전하세요."));
    }
}

void AMainWeapon::PerformLineTrace()
{
    FVector Start = WeaponMesh->GetComponentLocation();
    FVector ForwardVector = WeaponMesh->GetForwardVector();
    FVector End = Start + (ForwardVector * Range);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

    if (bHit)
    {
        UGameplayStatics::ApplyPointDamage(
            HitResult.GetActor(),
            Damage,
            ForwardVector,
            HitResult,
            GetInstigatorController(),
            this,
            nullptr
        );
    }

    // 디버그 라인 표시
    if (bDebugLine)
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
    }
}

void AMainWeapon::ActivateMuzzle()
{
    if (MuzzleEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMesh, TEXT("MuzzleSocket"));
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
}

void AMainWeapon::StartFire()
{
    Fire();
}

void AMainWeapon::StopFire()
{
}

void AMainWeapon::Reload()
{
    if (AmmoCount < MaxAmmo)
    {
        bCanFire = false;
        UE_LOG(LogTemp, Warning, TEXT("재장전 중..."));

        GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMainWeapon::FinishReload, ReloadTime, false);
    }
}

void AMainWeapon::FinishReload()
{
    AmmoCount = MaxAmmo;
    bCanFire = true;
    UE_LOG(LogTemp, Warning, TEXT("재장전 완료, 탄약 : %d"), AmmoCount);
}