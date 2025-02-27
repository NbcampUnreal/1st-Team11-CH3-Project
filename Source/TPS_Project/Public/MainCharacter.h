// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "MainCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class TPS_PROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
    UNiagaraComponent* MuzzleFlash;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float NormalSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* ReloadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
	bool IsFire;

	FTimerHandle FireTimer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
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
	UFUNCTION ()
	void StopGunFire (const FInputActionValue& Value);

	UFUNCTION()
	void Reload(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void PlayDamageAnim();

    virtual float TakeDamage(float DamageAmount,
        struct FDamageEvent const &DamageEvent,
        AController *EventInstigator,
        AActor *DamageCauser) override;

	float GetCharacterHealth() const;

	void SetCharacterHealth(float Value);
	
	void InitAnimation();

	void Fire();
	void Temp();
	void TestFire();
private:
	float Health;
	int Ammo;
};
