// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUCharacterBase.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASTUCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
