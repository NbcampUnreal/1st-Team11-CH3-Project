// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFireAnimNotify.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGunFireAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(Cast<AMainCharacter>(MeshComp->GetOwner()))) return;

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (MainCharacter)
	{
		MainCharacter->ActivateMuzzle();
	}
}
