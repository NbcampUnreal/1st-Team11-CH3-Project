#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetMesh(), TEXT("Rifle"));
	//FAttachmentTransformRules AttachmentRules ( EAttachmentRule::SnapToTarget , true );
	StaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("RightHandRifle")));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	NormalSpeed = 600.0f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
    InitAnimation();
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector MoveInput = Value.Get<FVector>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector() * NormalSpeed, MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector() * NormalSpeed, MoveInput.Y);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	SpringArm->SetRelativeRotation(FRotator(LookInput.Y + SpringArm->GetDesiredRotation().Pitch, 0, 0));
	//SpringArm->SetRelativeRotation(FRotator(0, LookInput.X + SpringArm->GetDesiredRotation().Yaw, 0));
	AddControllerPitchInput(LookInput.Y);
}

void AMainCharacter::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void AMainCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void AMainCharacter::GunFire(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		IsFire = Value.Get<bool>();
		Fire();
	}
}

void AMainCharacter::StopGunFire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp ,Warning ,TEXT("Stop Fire"));
	IsFire = Value.Get<bool>();
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void AMainCharacter::Reload(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
	UAnimInstance* AnimInstance=GetMesh()->GetAnimInstance();

	if ( AnimInstance && ReloadMontage )
	{
		AnimInstance->Montage_Play(ReloadMontage);
	}
}

float AMainCharacter::GetCharacterHealth() const
{
	return Health;
}

void AMainCharacter::SetCharacterHealth(float Value)
{
	Health += Value;
}

void AMainCharacter::InitAnimation()
{
	if (FireMontage)
	{
        const auto &NotifyEvents = FireMontage->Notifies;

        for (auto &Notify : NotifyEvents)
		{
            
		}
	}
}

void AMainCharacter::PlayDamageAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("Play Damage Anim"));
	UAnimInstance* AnimInstance=GetMesh()->GetAnimInstance();

	if (AnimInstance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
	}
}

void AMainCharacter::Fire()
{
	GetWorldTimerManager().SetTimer(FireTimer , this , &AMainCharacter::Temp, 0.2f , true );
}

void AMainCharacter::Temp()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Setup"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController());

	EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
	EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::StartJump);
	EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &AMainCharacter::StopJump);
	EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
	EnhancedInput->BindAction(PlayerController->GunFireAction, ETriggerEvent::Triggered, this, &AMainCharacter::GunFire);
	EnhancedInput->BindAction(PlayerController->GunFireAction , ETriggerEvent::Completed , this , &AMainCharacter::StopGunFire);
	EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered, this, &AMainCharacter::Reload);
}

