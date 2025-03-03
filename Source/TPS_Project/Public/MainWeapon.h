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

    virtual void StartFiring();
    virtual void StopFiring();
    virtual void Fire();

protected:
    virtual void BeginPlay() override;
    void LineTraceShoot();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* WeaponMesh; // 총기모델

    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category="Appearance")
    UMaterialInterface* WeaponMaterial; // 머터리얼

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Stats")
    float Damage = 25.f; // 총기 데미지

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Stats")
    float Range = 2000.0f; // 총기 사거리

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Stats")
    float FireRate = 0.1f; // 총기 연사속도

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effects")
    UParticleSystem* MuzzleFlash; // 총구 화염 이펙트

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effects")
    UParticleSystem* ImpactEffect; // 피격 이펙트

private:
    UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    bool bDebugLine = true;

    FTimerHandle FireTimerHandle;

};
