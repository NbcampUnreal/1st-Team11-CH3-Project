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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target|Health")
	float Health;


    FTimerHandle HUDUpdateTimerHandle;

public:
    float GetTargetHealth() const;
	
protected:
    virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void OnTargetDestroy();
    void UpdateOverheadHP();

};


/** ================= 1. UPROPERTY (리플렉션 시스템 변수) ================= **/

/** ================= 2. UFUNCTION (리플렉션 시스템 함수) ================= **/

/** ================= 3. 생성자 및 필수 오버라이드 함수 ================= **/

/** ================= 4. 일반 메서드 (리플렉션이 필요 없는 함수) ================= **/

/** ================= 5. 일반 멤버 변수 (리플렉션이 필요 없는 변수) ================= **/
