// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageAnimNotifyState.h"
#include "MainCharacter.h"

void UDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!IsValid(Cast<AMainCharacter>(MeshComp->GetOwner()))) return;

    AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());

    if (MainCharacter)
    {
        MainCharacter->SetDamageState(true);
    }
}

void UDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!IsValid(Cast<AMainCharacter>(MeshComp->GetOwner()))) return;

    AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());

    if (MainCharacter)
    {
        MainCharacter->SetDamageState(false);
    }
}