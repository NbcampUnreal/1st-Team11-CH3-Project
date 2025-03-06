#include "MainWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"



AMainWeapon::AMainWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    // 스태틱 메시 생성
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    MuzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
    MuzzleFlash->SetupAttachment(WeaponMesh);

    FireRate = 0.1f;
}

void AMainWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void AMainWeapon::StartFire()
{
    if (AmmoCount > 0 && bCanFire)
    {
        PerformLineTrace();
        ActivateSoundParticle();
        AmmoCount--;
        UE_LOG(LogTemp, Warning, TEXT("남은 탄약 : %d"), AmmoCount);
    }
    else
    {
        UGameplayStatics::PlaySoundAtLocation(this, EmptyAmmoSound, GetActorLocation());
    }
}

void AMainWeapon::PerformLineTrace()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

    if (!PlayerController) return;

    FVector CameraLocation;
    FRotator CameraRotation;

    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector Direction = CameraRotation.Vector();

    float Distance = 10000.f;

    FVector EndLocation = CameraLocation + (Direction * Distance);

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        CameraLocation,
        EndLocation,
        ECC_Visibility, 
        TraceParams
    );

    if (bHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Object: %s"), *HitResult.GetActor()->GetName());

        if (ShotImpact)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                ShotImpact,
                HitResult.ImpactPoint,
                HitResult.ImpactNormal.Rotation(),
                FVector(1.0f),
                true
            );
        }

        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f);
        AActor* HitActor = HitResult.GetActor();
        if (HitActor->ActorHasTag("Enemy"))
        {
            UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, this, UDamageType::StaticClass());
        }
    }

    //DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Blue, false, 2.0f, 0, 1.0f);
}

void AMainWeapon::Reload()
{
    //if (AmmoCount < MaxAmmo)
    //{
    bCanFire = false;
    UE_LOG(LogTemp, Warning, TEXT("재장전 중..."));
    UGameplayStatics::PlaySoundAtLocation(this, GunReloadSound, GetActorLocation());
    GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMainWeapon::FinishReload, ReloadTime, false);
    //}
}

float AMainWeapon::GetFireRate() const
{
    return FireRate;
}

void AMainWeapon::FinishReload()
{
    AmmoCount = MaxAmmo;
    bCanFire = true;
    UE_LOG(LogTemp, Warning, TEXT("재장전 완료, 탄약 : %d"), AmmoCount);
}

void AMainWeapon::ActivateSoundParticle()
{
    UE_LOG(LogTemp, Warning, TEXT("Activate Particle"));
    MuzzleFlash->Activate();
    UGameplayStatics::PlaySoundAtLocation(this, GunFireSound, GetActorLocation());
}

int AMainWeapon::GetMaxAmmo() const
{
    return MaxAmmo;
}

int AMainWeapon::CurrentAmmo() const
{
    return AmmoCount;
}
