#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetLightMachine.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class TPS_PROJECT_API ATargetLightMachine : public AActor
{
	GENERATED_BODY()

    // 스크린 모드?
    // 월드 모드? 
	
public:	
	ATargetLightMachine();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target|Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Target|Component")
	UBoxComponent* BoxCollision;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* OverheadWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Target|Health")
	float MaxHealth;
	float Health;
	
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void OnTargetDestroy();
    void UpdateOverheadHP();

};
