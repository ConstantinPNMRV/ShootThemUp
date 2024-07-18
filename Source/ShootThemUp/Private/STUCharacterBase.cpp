// Fill out your copyright notice in the Description page of Project Settings.

#include "STUCharacterBase.h"

ASTUCharacterBase::ASTUCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTUCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASTUCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
