// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "SSCommonGameModeState.generated.h"

class USSGameModeManagerComponent;
/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API ASSCommonGameModeState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	ASSCommonGameModeState();
	
protected:
	// Handles loading and managing the current gameplay experience
	UPROPERTY()
	TObjectPtr<USSGameModeManagerComponent> GameModeManagerComponent;
	
};

