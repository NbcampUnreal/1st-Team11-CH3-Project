// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class TPS_PROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	TArray<AActor*> PatrolPoints; 
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
    float MaxHP = 50.f;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
    float DamageAmoun = 10;
    UPROPERTY(EditDefaultsOnly, Category = Hitbox)
    USphereComponent *LeftHitbox;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
    bool DieCheker = false;
    UPROPERTY(EditDefaultsOnly, Category = Hitbox)
    USphereComponent *RightHitbox;
    UFUNCTION(BlueprintCallable)
    void SetLeftHitbox(ECollisionEnabled::Type CollisionEnabled); 
    UFUNCTION(BlueprintCallable)
    void SetRightHitbox(ECollisionEnabled::Type CollisionEnabled);
    UFUNCTION(BlueprintCallable)
    void DealDamage(AActor* OtherActor);
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    const FName GetZombieType() const
    {
        return ZombieType;
    }

    void Die();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    FName ZombieType = "CommonZombie";
    float CurrentHP = MaxHP;
    FTimerHandle DeathTimerHandle;
    void DestroyMyActor();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
