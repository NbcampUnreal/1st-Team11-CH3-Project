#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetLightMachine.generated.h"

UCLASS()
class TPS_PROJECT_API ATargetLightMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetLightMachine();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target|Component")
	UStaticMeshComponent* StaticMeshComp;

	float Health;
	float MaxHealth;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void OnTargetDestroy();

};
