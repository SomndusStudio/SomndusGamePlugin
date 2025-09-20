// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSNarrativeAgentComponent.h"

#include "Narrative/SSNarrativeContext.h"
#include "Narrative/SSNarrativeGraph.h"


// Sets default values for this component's properties
USSNarrativeAgentComponent::USSNarrativeAgentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USSNarrativeAgentComponent::BeginPlay()
{
	Super::BeginPlay();
	LocalNarrativeContext = NewObject<USSNarrativeContext>(this);
}


USSNarrativeContext* USSNarrativeAgentComponent::GetLocalNarrativeContext()
{
	if (!LocalNarrativeContext)
	{
		LocalNarrativeContext = NewObject<USSNarrativeContext>(this);

		if (!ProfileId.IsNone())
		{
			FSSNarrativeProfile NewProfile;
			NewProfile.ProfileId = ProfileId;

			LocalNarrativeContext->NarrativeProfiles.Add(NewProfile);
		}
	}

	return LocalNarrativeContext;
}

void USSNarrativeAgentComponent::EvaluateNarrative(float DeltaTime)
{
	EvaluationTimer += DeltaTime;
	if (EvaluationTimer < EvaluationInterval) return;

	EvaluationTimer = 0.0f;

	for (USSNarrativeGraph* Graph : AvailableNarrativeGraphs) {
		if (Graph && Graph->GetRootNode() && Graph->GetRootNode()->CanActivate(LocalNarrativeContext)) {
			TryStartNarrative(Graph);
			break; // Un seul à la fois
		}
	}
}

bool USSNarrativeAgentComponent::TryStartNarrative(USSNarrativeGraph* Graph)
{
	if (!Graph || !Graph->GetRootNode()) return false;

	// Execute ce graphe en local
	Graph->GetRootNode()->NativeExecute(LocalNarrativeContext, NarrativeManager);
	return true;
}
