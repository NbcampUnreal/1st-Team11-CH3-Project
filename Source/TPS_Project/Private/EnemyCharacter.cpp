// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = false;


    LeftHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHitbox"));
    RightHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("RightHitbox"));

    LeftHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHand"));
    RightHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHand"));
    LeftHitbox->SetSphereRadius(40.f);
    RightHitbox->SetSphereRadius(40.f);

    LeftHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AEnemyCharacter::SetLeftHitbox(ECollisionEnabled::Type CollisionEnabled)
{
    if (LeftHitbox) //널 체크
        LeftHitbox->SetCollisionEnabled(CollisionEnabled);
}

void AEnemyCharacter::SetRightHitbox(ECollisionEnabled::Type CollisionEnabled)
{
    if (RightHitbox) //널 체크
        RightHitbox->SetCollisionEnabled(CollisionEnabled);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::DealDamage(AActor* OtherActor)
{
    if (OtherActor)
    {
        UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetController(), this, UDamageType::StaticClass());
    }
}
// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

