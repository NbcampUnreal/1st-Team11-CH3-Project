#include "BossLevelDoor.h"
#include "Components/BoxComponent.h"

ABossLevelDoor::ABossLevelDoor()
{
	PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    Door->SetupAttachment(Scene);

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(Scene);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossLevelDoor::OnOverlapBegin);

    LerpAlpha = 0.0f;
    bIsClosed = true;
}

void ABossLevelDoor::BeginPlay()
{
    Super::BeginPlay();

    StartRotation = GetActorRotation();
    EndRotation = StartRotation + FRotator(0.0f, -150.0f, 0.0f);
}

void ABossLevelDoor::OnOverlapBegin(UPrimitiveComponent* OverllapedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bIsClosed)
    {
        if (OtherActor && OtherActor->ActorHasTag("Player"))
        {
            bIsClosed = false;
            GetWorld()->GetTimerManager().SetTimer(DoorTimerHandle, this, &ABossLevelDoor::OpenDoor, 0.02f, true);
        }
    }
}

void ABossLevelDoor::OpenDoor()
{
    LerpAlpha += 0.01f;
    if (LerpAlpha >= 1.0f)
    {
        GetWorldTimerManager().ClearTimer(DoorTimerHandle);

        LerpAlpha = 0.0f;
        GetWorldTimerManager().SetTimer(DoorTimerHandle, this, &ABossLevelDoor::CloseDoor, 0.02f, true, 5.0f);
        return;
    }

    FRotator NewRotation = FMath::Lerp(StartRotation, EndRotation, LerpAlpha);
    SetActorRotation(NewRotation);    
}

void ABossLevelDoor::CloseDoor()
{
    LerpAlpha += 0.01f;
    if (LerpAlpha >= 1.0f)
    {
        GetWorldTimerManager().ClearTimer(DoorTimerHandle);

        LerpAlpha = 0.0f;
        bIsClosed = true;
        return;
    }

    FRotator NewRotation = FMath::Lerp(EndRotation, StartRotation, LerpAlpha);
    SetActorRotation(NewRotation);
}

