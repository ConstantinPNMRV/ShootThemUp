// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameModeBase.h"
#include "Player/STUCharacterBase.h"
#include "Player/STUPlayerController.h"

ASTUGameModeBase::ASTUGameModeBase()
{
	DefaultPawnClass = ASTUCharacterBase::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
}
