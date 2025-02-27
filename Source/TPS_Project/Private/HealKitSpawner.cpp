#include "HealKitSpawner.h"
#include "HealKit.h"

AHealKitSpawner::AHealKitSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(Scene);

	// ConstructorHelpers::FClassFinder는 생성자에서만 사용 가능
	static ConstructorHelpers::FClassFinder<AHealKit> BPHealKit(TEXT("/Game/Blueprints/BP_HealKit.BP_HealKit_C"));
	if (BPHealKit.Succeeded())
	{
		HealKitClass = BPHealKit.Class;
	}

	SpawnedHealKit = nullptr;
	SpawnInterval = 3.0f;
	bIsTimerSet = false;
}

void AHealKitSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AHealKitSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TWeakObjectPtr<AHealKit> WeakSpawnedHealKit = SpawnedHealKit;

	// 스폰된 힐팩이 없고 타이머 설정 안 한 경우만
	if (!WeakSpawnedHealKit.IsValid() && !bIsTimerSet)
	{
		bIsTimerSet = true;

		FTimerHandle SpawnTimerHandle;
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AHealKitSpawner::SpawnHealKit, SpawnInterval, false);
	}
}

void AHealKitSpawner::SpawnHealKit()
{
	if (HealKitClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Heal Kit!!"));

		FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 120.0f);
		SpawnedHealKit = GetWorld()->SpawnActor<AHealKit>(HealKitClass, SpawnLocation, GetActorRotation());

		bIsTimerSet = false;
	}
}