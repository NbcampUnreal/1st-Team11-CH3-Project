// Fill out your copyright notice in the Description page of Project Settings.


#include "TracerProjectile.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

// Sets default values
ATracerProjectile::ATracerProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    TracerEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TracerEffect"));
    SetRootComponent(TracerEffect);
    // RootComponent = TracerEffect;

}

void ATracerProjectile::InitTracer(const FVector& StartPos, const TArray<FVector>& EndPos)
{
    //SetActorLocation(StartPos);

    if (TracerEffect)
    {
        TracerEffect->Activate();

        TracerEffect->SetVariablePosition(TEXT("User.MuzzlePosition"), StartPos);
        UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(TracerEffect, TEXT("User.ImpactPositions"), EndPos);
        TracerEffect->SetNiagaraVariableBool(TEXT("User.Trigger"), true);
    }
}

// Called when the game starts or when spawned
void ATracerProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (!TracerEffect->GetAsset())
    {
        UE_LOG(LogTemp, Error, TEXT("TracerEffect does not have a valid Niagara system!"));
    }
}


