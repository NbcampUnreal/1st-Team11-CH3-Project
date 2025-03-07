// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TracerProjectile.generated.h"

UCLASS()
class TPS_PROJECT_API ATracerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ATracerProjectile();
    void InitTracer(const FVector& StartPos, const TArray<FVector>& EndPos);

protected:

	virtual void BeginPlay() override;
private:
    UPROPERTY(VisibleAnywhere)
    UNiagaraComponent* TracerEffect;
    

    FVector TargetLocation;

    UPROPERTY(EditAnywhere, Category = "Tracer")
    float Speed = 10000.0f;  // 탄환 속도
public:	
};
