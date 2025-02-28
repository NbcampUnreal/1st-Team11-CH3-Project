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

    UPROPERTY(EditDefaultsOnly, Category = Hitbox)
    USphereComponent *LeftHitbox;

    UPROPERTY(EditDefaultsOnly, Category = Hitbox)
    USphereComponent *RightHitbox;
    UFUNCTION(BlueprintCallable)
    void SetLeftHitbox(ECollisionEnabled::Type CollisionEnabled); 
    UFUNCTION(BlueprintCallable)
    void SetRightHitbox(ECollisionEnabled::Type CollisionEnabled);

    const FName GetZombieType() const
    {
        return ZombieType;
    }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    FName ZombieType = "CommonZombie";
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
