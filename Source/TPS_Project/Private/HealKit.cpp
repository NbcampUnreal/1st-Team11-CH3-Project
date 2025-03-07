#include "HealKit.h"
#include "MainCharacter.h"

AHealKit::AHealKit()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemType = "HealKit";
	Heal = 40.0f;
	RotationSpeed = 90.0f;

	MedBagPatch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MedBagPatch"));
	MedBagPatch->SetupAttachment(StaticMeshComp);

	Scissors = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scissors"));
	Scissors->SetupAttachment(StaticMeshComp);

}

void AHealKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}

void AHealKit::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AMainCharacter* Player = Cast<AMainCharacter>(Activator))
		{
			Player->SetCharacterHealth(Heal);
		}

		DestroyItem();
	}
}


