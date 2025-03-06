#include "TargetLightMachine.h"
#include "MainGameState.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

ATargetLightMachine::ATargetLightMachine()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(Scene);

	BoxCollision=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Scene);

	MaxHealth = 5000.0f;
	Health = MaxHealth;


    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(StaticMeshComp/*GetMesh()*/);
    OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen); // 스크린모드 


    Tags.Add(TEXT("Target"));
}

void ATargetLightMachine::BeginPlay()
{
    Super::BeginPlay();
    
    // Wave 타이머 -> UpdateHUD로 변경; 매 시간? 마다 UpdateHUD해주기 
    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &ATargetLightMachine::UpdateOverheadHP,
        0.1f,//0.1초마다
        true
    );
}
float ATargetLightMachine::GetTargetHealth() const
{
    return Health;
}

float ATargetLightMachine::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (Health <= 0.0f)
	{
		OnTargetDestroy();
	}

	return ActualDamage;
}

void ATargetLightMachine::OnTargetDestroy()
{
	AMainGameState* MainGameState = GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
	if (MainGameState)
	{
		MainGameState->GameOver(false);
	}
}

void ATargetLightMachine::UpdateOverheadHP()
{
    if (!OverheadWidget) return; // 제대로 존재하는지..

    // 1) OverheadWidgetInstance가져옥
    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstance) return;

    // 2) HealthBar Progress 
    if (UProgressBar* HPBar = Cast<UProgressBar>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverheadHPBar"))))
    {
        // 범위는 0.0~1.0으로 해야함
        // Health / MaxHealth 
        float HPPercent = 0.f;
        if (MaxHealth > 0.f)
        {
            
            HPPercent = Health / MaxHealth;
        }
        HPBar->SetPercent(HPPercent);

        // HPPercent가 낮으면 빨갛게 
        if (HPPercent < 0.3f)
        {
            FLinearColor LowHPColor = FLinearColor::Red;
            HPBar->SetFillColorAndOpacity(LowHPColor);
        }
        else
        {
            FLinearColor LowHPColor = FLinearColor::Green;
            HPBar->SetFillColorAndOpacity(LowHPColor);
        }
    }

}

