// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUCharacterBase.generated.h"

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCharacterMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Compomemts")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Compomemts")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	UInputAction* SprintAction;

private:
	UPROPERTY()
	float MaxSpeed;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

public:
	ASTUCharacterBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	float GetMovementDirection() const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Sprint(const FInputActionValue& Value);
};
