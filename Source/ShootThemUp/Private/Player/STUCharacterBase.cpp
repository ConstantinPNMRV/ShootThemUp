// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/STUCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/DamageEvents.h"

ASTUCharacterBase::ASTUCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CharacterMovementComponent = GetCharacterMovement();
	MaxSpeed = CharacterMovementComponent->MaxWalkSpeed;

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
}

void ASTUCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	check(HealthComponent);
	check(HealthTextComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ASTUCharacterBase::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUCharacterBase::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ASTUCharacterBase::OnGroundLanded);
}

void ASTUCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTUCharacterBase::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTUCharacterBase::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASTUCharacterBase::Sprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASTUCharacterBase::Sprint);
	}
}

float ASTUCharacterBase::GetMovementDirection() const
{
	if (GetVelocity().IsZero())
	{
		return 0;
	}

	const FVector VelocityNormal = GetVelocity().GetSafeNormal();
	const float AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const float Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUCharacterBase::Move(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();

	if (AxisValue.X == 0 && AxisValue.Y == 0)
	{
		return;
	}

	AddMovementInput(GetActorForwardVector(), AxisValue.Y);
	AddMovementInput(GetActorRightVector(), AxisValue.X);
}

void ASTUCharacterBase::Look(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();

	AddControllerPitchInput(AxisValue.Y);
	AddControllerYawInput(AxisValue.X);
}

void ASTUCharacterBase::Sprint(const FInputActionValue& Value)
{
	if (Value.Get<bool>() == true)
	{
		CharacterMovementComponent->MaxWalkSpeed = MaxSpeed * 2;
	}
	else
	{
		CharacterMovementComponent->MaxWalkSpeed = MaxSpeed;
	}
}

void ASTUCharacterBase::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10, FColor::Red, FString::Printf(TEXT("Is Dead")));

	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOfDeath);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

}

void ASTUCharacterBase::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUCharacterBase::OnGroundLanded(const FHitResult& Hit)
{
	const float FallVelocityZ = -GetVelocity().Z;

	if (FallVelocityZ < LandedDamageVelocity.X)
	{
		return;
	}

	const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}
