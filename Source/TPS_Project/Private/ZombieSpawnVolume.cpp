#include "ZombieSpawnVolume.h"
#include "Components/BoxComponent.h"

AZombieSpawnVolume::AZombieSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	bIsSpawn = false;
	SpawnInterval = 7.0f;

	ZombieDataTable = nullptr;

	TotalDeltaTime = 0.0f;
}

void AZombieSpawnVolume::SpawnRandomZombie()
{
	FZombieSpawnRow* RandomZombie = GetRandomZombie();
	if (RandomZombie)
	{
		UClass* SpawnItemClass = RandomZombie->ZombieClass;
		if (SpawnItemClass)
		{
			SpawnZombie(SpawnItemClass);
		}
	}
}

void AZombieSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSpawn)
	{
		if (TotalDeltaTime >= SpawnInterval)
		{
			SpawnRandomZombie();

			TotalDeltaTime = 0.0f;
		}
	}

	TotalDeltaTime += DeltaTime;
}

FVector AZombieSpawnVolume::GetRandomPointToSpawn() const
{
	const FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	const FVector BoxLocation = SpawningBox->GetComponentLocation();

	FVector RandomPoint = BoxLocation + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		0.0f
	);

	return RandomPoint;
}

FZombieSpawnRow* AZombieSpawnVolume::GetRandomZombie() const
{
	if (!ZombieDataTable)
	{
		return nullptr;
	}

	// 모든 데이터(Row) 가져오기
	TArray<FZombieSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ZombieSpawnText"));
	ZombieDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty())
	{
		return nullptr;
	}

	// 전체 스폰 확률 구하기
	float TotalChance = 0.0f;
	for (const FZombieSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	float RandomValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	for (FZombieSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			AccumulateChance += Row->SpawnChance;
			if (RandomValue <= AccumulateChance)
			{
				return Row;
			}
		}
	}

	return nullptr;
}

void AZombieSpawnVolume::SpawnZombie(TSubclassOf<AActor> ZombieClass)
{
	if (ZombieClass)
	{
		GetWorld()->SpawnActor<AActor>(ZombieClass, GetRandomPointToSpawn(), FRotator::ZeroRotator);
	}
}
