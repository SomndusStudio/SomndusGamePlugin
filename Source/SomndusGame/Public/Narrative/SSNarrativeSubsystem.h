// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSNarrativeContext.h"
#include "SSNarrativeManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SSNarrativeSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSNarrativeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/** Initializes the global narrative context */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Returns the global narrative context */
	UFUNCTION(BlueprintCallable)
	USSNarrativeContext* GetGlobalContext() const;

	/** Returns the central narrative manager */
	UFUNCTION(BlueprintCallable, Category="Narrative")
	USSNarrativeManager* GetNarrativeManager() const;

private:
	UPROPERTY()
	USSNarrativeContext* GlobalContext;
	
	UPROPERTY()
	USSNarrativeManager* NarrativeManager;
};
