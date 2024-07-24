// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCharacterMovementComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class UDamageType;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOfDeath = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(500, 2000);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10, 100);

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputMappingContext* InputMappingMoving;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputMappingContext* InputMappingWeapons;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* ShotAction;

private:
	UPROPERTY()
	float MaxSpeed;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

public:
	ASTUBaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	float GetMovementDirection() const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Category = "Movement")
	void Move(const FInputActionValue& Value);

	UFUNCTION(Category = "Movement")
	void Look(const FInputActionValue& Value);

	UFUNCTION(Category = "Movement")
	void Sprint(const FInputActionValue& Value);

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnHealthChanged(float Health);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
};
