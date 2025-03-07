// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/DamageType.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = false;
    this->Tags.Add(FName(TEXT("Enemy")));

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

void AEnemyCharacter::Die()
{
    DieCheker = true;
    {
        if (AAIController* AIController = Cast<AAIController>(GetController()))
        {
            AIController->StopMovement();
            if (UBrainComponent* BrainComp = AIController->BrainComponent)
            {
                BrainComp->StopLogic("Die 함수 호출로 인한 정지");
            }
        }
    }
    GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyCharacter::DestroyMyActor, 5.f, false);
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
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Target"))
        {
            UGameplayStatics::ApplyDamage(OtherActor, DamageAmoun, GetController(), this, UDamageType::StaticClass());
        }
    }
}
float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // 기본 데미지 처리(옵션에 따라 부모 클래스를 호출할지 결정)
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage <= 0.f)
    {
        return 0.f;
    }

    // 체력 차감
    CurrentHP -= ActualDamage;

    // 체력이 0 이하이면 죽음 처리
    if (CurrentHP <= 0.f)
    {
        Die();
    }

    return ActualDamage;
}

void AEnemyCharacter::DestroyMyActor()
{
    Destroy();
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

