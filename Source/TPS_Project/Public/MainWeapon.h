#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainWeapon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class ATracerProjectile;



UCLASS()
class TPS_PROJECT_API AMainWeapon : public AActor
{
    GENERATED_BODY()

public:
    AMainWeapon();
    void StartFire();
    void Reload();
    float GetFireRate() const;
    void ActivateSoundParticle(const FVector& StartPos, const FVector EndPos);
    int GetMaxAmmo() const;
    int CurrentAmmo() const;
protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun Stats")
    float Damage = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun Stats")
    float Range = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
    float FireRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    int AmmoCount = 30;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    int MaxAmmo = 30;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    float ReloadTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    UNiagaraComponent* MuzzleFlash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    UNiagaraComponent* ShotTracerComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    UNiagaraSystem* ShotImpact;



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    UNiagaraSystem* ShotTracerSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    TSubclassOf<ATracerProjectile> TracerClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* GunFireSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* GunReloadSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* EmptyAmmoSound;

    //UFUNCTION(BlueprintCallable, Category = "LineTrace")
    //void ShootLineTrace();
   

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    bool bDebugLine = true;

private:
    void PerformLineTrace();
    void FinishReload();
   

    bool bCanFire = true;
    FTimerHandle ReloadTimer;

};