// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BezierProjectile.generated.h"

UCLASS()
class TPS_PROJECT_API ABezierProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABezierProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene")
    USceneComponent* SceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMesh")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector StartPoint;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector ControlPoint1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector ControlPoint2;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector EndPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TimeElapsed;

    FVector CalculateBezierPoint(float t, FVector P0, FVector P1, FVector P2, FVector P3);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
