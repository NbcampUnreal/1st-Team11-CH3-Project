
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "MainCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class AMainWeapon;
struct FInputActionValue;

UCLASS()
class TPS_PROJECT_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMainCharacter();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    UCameraComponent* Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    USpringArmComponent* SpringArm;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    //UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    AMainWeapon* MainWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    TSubclassOf<AMainWeapon> TestWeapon;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    //UNiagaraComponent* MuzzleFlash;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    float NormalSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* FireMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* HitMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* ReloadMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathMontage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsFire;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bHasDamage;

    FTimerHandle FireTimer;



public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void Move(const FInputActionValue& Value);
    UFUNCTION()
    void Look(const FInputActionValue& Value);
    UFUNCTION()
    void StartJump(const FInputActionValue& Value);
    UFUNCTION()
    void StopJump(const FInputActionValue& Value);
    UFUNCTION()
    void GunFire(const FInputActionValue& Value);
    UFUNCTION()
    void StopGunFire(const FInputActionValue& Value);
    UFUNCTION()
    void Reload(const FInputActionValue& Value);
    UFUNCTION()
    void TestEquipWeapon();

    UFUNCTION(BlueprintCallable)
    void PlayDamageAnim();

    virtual float TakeDamage(float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser) override;

    float GetCharacterHealth() const;

    void SetCharacterHealth(float Value);

    void Fire();
    void GunShotAnimation();
    void ActivateMuzzle();
    void SetDamageState(bool HasDamage);
    void GameOver();

private:
    float Health;
    int Ammo;
    bool bIsGameOver;
};
