#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "MainWeapon.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCharacter::AMainCharacter() : MainWeapon(nullptr)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //MainWeapon = CreateDefaultSubobject<AMainWeapon>(TEXT("MainWeapon"));
    //MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("RightHandRifle")));

    //StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    //StaticMesh->SetupAttachment(GetMesh(), TEXT("Rifle"));
    ////FAttachmentTransformRules AttachmentRules ( EAttachmentRule::SnapToTarget , true );
    //StaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("RightHandRifle")));

    //MuzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
    //MuzzleFlash->SetupAttachment(StaticMesh);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    NormalSpeed = 600.0f;
    bHasDamage = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
    TestEquipWeapon();
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
    if (Value.Get<bool>() && !bHasDamage)
    {
        bIsFire = Value.Get<bool>();

        //GetWorldTimerManager().SetTimer(FireTimer, this, &AMainCharacter::GunShotAnimation, 0.2f, true);
        MainWeapon->Fire();
    }
}

void AMainCharacter::StopGunFire(const FInputActionValue& Value)
{
    bIsFire = Value.Get<bool>();
    //GetWorldTimerManager().ClearTimer(FireTimer);
}

void AMainCharacter::Reload(const FInputActionValue& Value)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance && ReloadMontage)
    {
        AnimInstance->Montage_Play(ReloadMontage);
    }
}

void AMainCharacter::TestEquipWeapon()
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
    PlayDamageAnim();
    return DamageAmount;
}

void AMainCharacter::Fire()
{
    GetWorldTimerManager().SetTimer(FireTimer, this, &AMainCharacter::GunShotAnimation, 0.1f, true);
}

void AMainCharacter::GunShotAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance && FireMontage)
    {
        AnimInstance->Montage_Play(FireMontage);
    }
}

// 총기 클래스로 옮겨갈 수도 있음
void AMainCharacter::ActivateMuzzle()
{
    //MuzzleFlash->Activate();
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
    }
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
    EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered, this, &AMainCharacter::Reload);
}

