#include "MainWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TracerProjectile.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"



AMainWeapon::AMainWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    // 스태틱 메시 생성
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    MuzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
    MuzzleFlash->SetupAttachment(WeaponMesh);

    ShotTracerComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Tracer"));
    ShotTracerComponent->SetupAttachment(WeaponMesh);

    FireRate = 0.1f;
}

void AMainWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (ShotTracerComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShotTracerComponent Initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShotTracerComponent is NOT Initialized"));
    }
}

void AMainWeapon::StartFire()
{
    if (AmmoCount > 0 && bCanFire)
    {
        PerformLineTrace();

        AmmoCount--;
        //UE_LOG(LogTemp, Warning, TEXT("남은 탄약 : %d"), AmmoCount);
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

    ActivateSoundParticle(CameraLocation + FVector(700, 0, 0), CameraLocation + FVector(350, 0, 0) + (Direction * Distance));
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

        AActor* HitActor = HitResult.GetActor();
        if (HitActor->ActorHasTag("Enemy"))
        {
            UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, this, UDamageType::StaticClass());
        }
    }
}

void AMainWeapon::Reload()
{
    bCanFire = false;
    UE_LOG(LogTemp, Warning, TEXT("재장전 중..."));
    UGameplayStatics::PlaySoundAtLocation(this, GunReloadSound, GetActorLocation());
    GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMainWeapon::FinishReload, ReloadTime, false);
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

void AMainWeapon::ActivateSoundParticle(const FVector& StartPos, const FVector EndPos)
{
    MuzzleFlash->Activate();

    ShotTracerComponent->DeactivateImmediate();
   

    TArray<FVector> EndPoss;
    EndPoss.Add(EndPos);

    ShotTracerComponent->SetVariablePosition(TEXT("User.MuzzlePosition"), WeaponMesh->GetComponentLocation());
    UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(ShotTracerComponent, TEXT("User.ImpactPositions"), EndPoss);
    //ShotTracerComponent->SetVariablePosition(TEXT("User.ImpactPositions"), EndPos);
    ShotTracerComponent->SetNiagaraVariableBool(TEXT("User.Trigger"), true);
    ShotTracerComponent->Activate();


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
