#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "MainWeapon.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter() : MainWeapon(nullptr)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    bIsGameOver = false;
    NormalSpeed = 600.0f;
    bHasDamage = false;
    Health = 100.f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
    EquipWeapon();
}



void AMainCharacter::Move(const FInputActionValue& Value)
{
    if (!Controller || bIsGameOver) return;
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
    if (Value.Get<bool>() || !bIsGameOver)
    {
        Jump();
    }
}

void AMainCharacter::StopJump(const FInputActionValue& Value)
{
    if (!Value.Get<bool>() || !bIsGameOver)
    {
        StopJumping();
    }
}

void AMainCharacter::GunFire(const FInputActionValue& Value)
{
    if (Value.Get<bool>() && !bHasDamage && !bIsGameOver)
    {
        bIsFire = Value.Get<bool>();

        GetWorldTimerManager().SetTimer(FireTimer, this, &AMainCharacter::GunShotAnimation, 0.2f, true);
        //MainWeapon->Fire();
    }
}

void AMainCharacter::StopGunFire(const FInputActionValue& Value)
{
    bIsFire = Value.Get<bool>();
    GetWorldTimerManager().ClearTimer(FireTimer);
}

void AMainCharacter::Reload(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Reloading"));
    if (bIsGameOver) return;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance && ReloadMontage)
    {
        AnimInstance->Montage_Play(ReloadMontage);
        MainWeapon->Reload();
    }
}

void AMainCharacter::EquipWeapon()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    MainWeapon = GetWorld()->SpawnActor<AMainWeapon>(TestWeapon);
    MainWeapon->SetOwner(this);
    MainWeapon->SetActorEnableCollision(false);
    MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("RightHandRifle")));

    FVector Location = FVector(0, -3.5f, 2.0f);
    FRotator Rotator = FRotator(80.f, 90.f, -75.f);
    MainWeapon->SetActorRelativeLocation(Location);
    MainWeapon->SetActorRelativeRotation(Rotator);

    FireRate = MainWeapon->GetFireRate();
}

float AMainCharacter::GetCharacterHealth() const
{
    return Health;
}

void AMainCharacter::SetCharacterHealth(float Value)
{
    Health += Value;
}

void AMainCharacter::PlayDamageAnim()
{
    if (bIsGameOver) return;

    GetWorldTimerManager().ClearTimer(FireTimer);
    bIsFire = false;
    bHasDamage = true;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance && HitMontage)
    {
        AnimInstance->StopAllMontages(1);
        AnimInstance->Montage_Play(HitMontage);
    }
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsGameOver) return 0.f;

    PlayDamageAnim();

    Health -= DamageAmount;

    if (Health <= 0)
    {
        Health = 0;
        GameOver();
    }

    UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);

    return DamageAmount;
}

void AMainCharacter::Fire()
{
    GetWorldTimerManager().SetTimer(FireTimer, this, &AMainCharacter::GunShotAnimation, FireRate, true);
}

void AMainCharacter::GunShotAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance && FireMontage)
    {
        AnimInstance->Montage_Play(FireMontage);
    }
}

void AMainCharacter::SetDamageState(bool HasDamage)
{
    bHasDamage = HasDamage;
}

void AMainCharacter::GameOver()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    
    if (AnimInstance && DeathMontage)
    {
        AnimInstance->Montage_Play(DeathMontage);
        UE_LOG(LogTemp, Warning, TEXT("Game Over"));
    }
}

int AMainCharacter::GetMaxAmmo()
{
    return MainWeapon->GetMaxAmmo();
}

int AMainCharacter::CurrentAmmo()
{
    return MainWeapon->CurrentAmmo();
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController());

    EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
    EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::StartJump);
    EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &AMainCharacter::StopJump);
    EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
    EnhancedInput->BindAction(PlayerController->GunFireAction, ETriggerEvent::Triggered, this, &AMainCharacter::GunFire);
    EnhancedInput->BindAction(PlayerController->GunFireAction, ETriggerEvent::Completed, this, &AMainCharacter::StopGunFire);
    EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Started, this, &AMainCharacter::Reload);
}

AMainWeapon* AMainCharacter::GetMainWeapon() const
{
    return MainWeapon;
}

