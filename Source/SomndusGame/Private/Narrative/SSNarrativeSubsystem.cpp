// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSNarrativeSubsystem.h"

void USSNarrativeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Instantiate and prepare the global context
	GlobalContext = NewObject<USSNarrativeContext>(this);

	// Initialize the narrative manager
	NarrativeManager = NewObject<USSNarrativeManager>(this);
	NarrativeManager->SetGlobalContext(GlobalContext);
}

USSNarrativeContext* USSNarrativeSubsystem::GetGlobalContext() const
{
	return GlobalContext;
}

USSNarrativeManager* USSNarrativeSubsystem::GetNarrativeManager() const
{
	return NarrativeManager;
}
