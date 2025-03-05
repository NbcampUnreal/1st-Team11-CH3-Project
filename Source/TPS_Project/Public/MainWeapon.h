#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainWeapon.generated.h"

UCLASS()
class TPS_PROJECT_API AMainWeapon : public AActor
{
    GENERATED_BODY()

public:
    AMainWeapon();
    virtual void Fire();
    void Reload();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun Stats")
    float Damage = 25.0f;

    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gun Stats")
    float Range = 5000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun Stats")
    float FireRate = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    int AmmoCount = 30;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    int MaxAmmo = 30;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    float ReloadTime = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
    UMaterialInterface* WeaponMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    bool bDebugLine = true;

private:
    void PerformLineTrace();
    void FinishReload();

    bool bCanFire = true;
    FTimerHandle ReloadTimer;

};