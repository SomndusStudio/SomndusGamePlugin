// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/SSCommonGameModeState.h"

#include "Mode/SSGameModeManagerComponent.h"

ASSCommonGameModeState::ASSCommonGameModeState()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GameModeManagerComponent = CreateDefaultSubobject<USSGameModeManagerComponent>(TEXT("GameModeManagerComponent"));
}
