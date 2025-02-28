#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossLevelDoor.generated.h"

class UBoxComponent;

UCLASS()
class TPS_PROJECT_API ABossLevelDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossLevelDoor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    UStaticMeshComponent* Door;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    UBoxComponent* TriggerBox;

    FTimerHandle DoorTimerHandle;

    FRotator StartRotation;
    FRotator EndRotation;
    float LerpAlpha;
    bool bIsClosed;

protected:
    virtual void BeginPlay() override;

    // Tick() 쓰지 않고 회전
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverllapedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void OpenDoor();
    void CloseDoor();
};
