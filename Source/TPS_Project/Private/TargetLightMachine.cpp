#include "TargetLightMachine.h"
#include "MainGameState.h"
#include "Components/BoxComponent.h"

ATargetLightMachine::ATargetLightMachine()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(Scene);

	BoxCollision=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Scene);

	MaxHealth = 5000.0f;
	Health = MaxHealth;
}

float ATargetLightMachine::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (Health <= 0.0f)
	{
		OnTargetDestroy();
	}

	return ActualDamage;
}

void ATargetLightMachine::OnTargetDestroy()
{
	AMainGameState* MainGameState = GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
	if (MainGameState)
	{
		MainGameState->GameOver();
	}
}

