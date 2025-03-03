#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelStreamer.generated.h"

class UBoxComponent;

UCLASS()
class TPS_PROJECT_API ALevelStreamer : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelStreamer();

protected:
	UPROPERTY(EditAnywhere)
	FName LevelToLoad;		// 로드할 레벨 이름

    bool bIsLoaded;

private:
	// 박스 컴포넌트에 overlap 되면 보스 레벨이 [스트림 인] 되도록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OverlapVolume;

protected:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
