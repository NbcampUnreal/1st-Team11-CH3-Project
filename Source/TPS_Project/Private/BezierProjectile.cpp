// Fill out your copyright notice in the Description page of Project Settings.


#include "BezierProjectile.h"

// Sets default values
ABezierProjectile::ABezierProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(SceneComponent);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(SceneComponent);

}

// Called when the game starts or when spawned
void ABezierProjectile::BeginPlay()
{
    Super::BeginPlay();

    StartPoint = GetActorLocation();
    ControlPoint1 = StartPoint + FVector(200, 0, 400); // 첫 번째 제어점 (곡선 조정)
    ControlPoint2 = StartPoint + FVector(400, 0, 800); // 두 번째 제어점
    EndPoint = StartPoint + FVector(1400, 0, 0); // 최종 도착 지점

    TimeElapsed = 0.0f;
}

FVector ABezierProjectile::CalculateBezierPoint(float t, FVector P0, FVector P1, FVector P2, FVector P3)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    FVector Point =
        (uuu * P0) +
        (3 * uu * t * P1) +
        (3 * u * tt * P2) +
        (ttt * P3);

    return Point;
}

// Called every frame
void ABezierProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TimeElapsed < 1.0f)
    {
        TimeElapsed += DeltaTime * 2.f; // 속도 조절 (0.5는 이동 속도)
        TimeElapsed = FMath::Clamp(TimeElapsed, 0.0f, 1.0f);

        // 베지어 곡선 공식 적용
        FVector NewLocation = CalculateBezierPoint(TimeElapsed, StartPoint, ControlPoint1, ControlPoint2, EndPoint);
        SetActorLocation(NewLocation);
    }

}

