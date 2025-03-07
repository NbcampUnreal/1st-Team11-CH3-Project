#include "OverlapBaseItem.h"
#include "Components/BoxComponent.h"

AOverlapBaseItem::AOverlapBaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollision->SetupAttachment(Scene);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(Scene);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AOverlapBaseItem::OnItemBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AOverlapBaseItem::OnItemEndOverlap);
}

void AOverlapBaseItem::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActivateItem(OtherActor);
	}
}

void AOverlapBaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AOverlapBaseItem::ActivateItem(AActor* Activator)
{

}

FName AOverlapBaseItem::GetItemType() const
{
	return ItemType;
}

void AOverlapBaseItem::DestroyItem()
{
	Destroy();
}
