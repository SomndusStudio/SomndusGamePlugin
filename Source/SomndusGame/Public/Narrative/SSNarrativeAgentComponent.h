// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSNarrativeManager.h"
#include "Components/ActorComponent.h"
#include "SSNarrativeAgentComponent.generated.h"


class USSNarrativeGraph;
class USSNarrativeContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOMNDUSGAME_API USSNarrativeAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USSNarrativeAgentComponent();

	UPROPERTY()
	USSNarrativeManager* NarrativeManager;
	
	/** Graphes narratifs disponibles pour cet agent */
	UPROPERTY(EditAnywhere, Category="Narrative")
	TArray<USSNarrativeGraph*> AvailableNarrativeGraphs;

	/** Tags de situation ou d’état que le PNJ utilise pour sa narration (mort, malade, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Narrative")
	FGameplayTagContainer AgentStateTags;

	/** Context narratif propre à cet agent */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Narrative")
	USSNarrativeContext* LocalNarrativeContext;

	/** Narrative profile identifier for this agent */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Narrative")
	FName ProfileId;
	
	/** Returns the local narrative context used by this agent */
	UFUNCTION(BlueprintCallable, Category="Narrative")
	USSNarrativeContext* GetLocalNarrativeContext();
	
	/** Tick ou event appelé pour vérifier les graphs activables */
	UFUNCTION(BlueprintCallable, Category="Narrative")
	void EvaluateNarrative(float DeltaTime);

	/** Déclenche un graphe spécifique si possible */
	UFUNCTION(BlueprintCallable, Category="Narrative")
	bool TryStartNarrative(USSNarrativeGraph* Graph);

protected:
	virtual void BeginPlay() override;

private:
	float EvaluationTimer = 0.0f;
	float EvaluationInterval = 1.0f; // Vérifie toutes les X secondes
};