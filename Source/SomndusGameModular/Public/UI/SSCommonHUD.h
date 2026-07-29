// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSCommonHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API ASSCommonHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	static FName NAME_HUDReady;

	ASSCommonHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PreInitializeComponents() override;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void OnShowHUDChange(IConsoleVariable* ConsoleVariable);
};
