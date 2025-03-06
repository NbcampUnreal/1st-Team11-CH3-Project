// Fill out your copyright notice in the Description page of Project Settings.


#include "TracerProjectile.h"

// Sets default values
ATracerProjectile::ATracerProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    TracerEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TracerEffect"));
    RootComponent = TracerEffect;

}

void ATracerProjectile::InitTracer(const FVector& StartPoint, const FVector& EndPoint)
{
    SetActorLocation(StartPoint);
    TargetLocation = EndPoint;

    // Niagara 파티클 활성화
    if (TracerEffect)
    {
        TracerEffect->Activate();
    }
}

// Called when the game starts or when spawned
void ATracerProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATracerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    float Distance = FVector::Dist(CurrentLocation, TargetLocation);

    if (Distance > 10.0f)  // 목표 지점까지 이동
    {
        FVector NewLocation = CurrentLocation + Direction * Speed * DeltaTime;
        SetActorLocation(NewLocation);
    }
    else
    {
        Destroy();  // 목표 지점 도달 시 삭제
    }
}

