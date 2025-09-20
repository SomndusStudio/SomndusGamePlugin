// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSNarrativeManager.h"

#include "Engine/GameInstance.h"
#include "GameFramework/Actor.h"
#include "Narrative/SSNarrativeAgentComponent.h"
#include "Narrative/SSNarrativeContext.h"
#include "Narrative/SSNarrativeSubsystem.h"

void USSNarrativeManager::SetGlobalContext(USSNarrativeContext* InContext)
{
	GlobalContext = InContext;
}

USSNarrativeContext* USSNarrativeManager::GetGlobalContext()
{
	if (!GlobalContext)
	{
		GlobalContext = GetGameInstance()->GetSubsystem<USSNarrativeSubsystem>()->GetGlobalContext();
	}

	return GlobalContext;
}

USSNarrativeContext* USSNarrativeManager::CreateContextFromProfileId(FName ProfileId)
{
	if (ProfileId.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateContextFromProfileId: ProfileId is None."));
		return nullptr;
	}

	// Attempt to get profile from global context
	const FSSNarrativeProfile* Profile = GetGlobalContext()->GetProfile(ProfileId);
	if (!Profile)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateContextFromProfileId: Profile %s not found."), *ProfileId.ToString());
		return nullptr;
	}

	USSNarrativeContext* NewContext = NewObject<USSNarrativeContext>(this);

	// Inject only the matching profile
	NewContext->NarrativeProfiles.Add(*Profile);

	return NewContext;
}


USSNarrativeContext* USSNarrativeManager::ResolveContext(const FSSContextBinding& Binding, UObject* RuntimeSource)
{
	const FGameplayTag& Tag = Binding.BindingTag;

	if (Tag.MatchesTagExact(SSGameplayTags::TAG_SS_CONTEXT_GLOBAL))
	{
		return GetGlobalContext();
	}
	else if (Tag.MatchesTag(SSGameplayTags::TAG_SS_CONTEXT_PROFILE))
	{
		return CreateContextFromProfileId(Binding.ContextQualifier);
	}
	else if (Tag.MatchesTagExact(SSGameplayTags::TAG_SS_CONTEXT_FROM_ACTOR))
	{
		if (AActor* Actor = Cast<AActor>(RuntimeSource))
		{
			if (auto* Comp = Actor->FindComponentByClass<USSNarrativeAgentComponent>())
			{
				return Comp->GetLocalNarrativeContext();
			}
		}
	}
	else if (Tag.MatchesTagExact(FGameplayTag::RequestGameplayTag("Context.Runtime.Provided")))
	{
		return Cast<USSNarrativeContext>(RuntimeSource);
	}

	UE_LOG(LogTemp, Warning, TEXT("Unable to resolve narrative context for tag %s"), *Tag.ToString());
	return nullptr;
}

void USSNarrativeManager::StartGraph(USSNarrativeGraph* Graph, UObject* RuntimeSource)
{
	if (!Graph || !Graph->AllNodes[0])
	{
		UE_LOG(LogTemp, Warning, TEXT("StartGraph: Invalid graph or entry node."));
		return;
	}

	NarrativeContext = ResolveContext(Graph->ContextBinding, RuntimeSource);
	if (!NarrativeContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartGraph: Failed to resolve context for graph %s."), *Graph->GetName());
		return;
	}
	
	// First node execution
	if (!Graph->AllNodes[0]->CanActivate(NarrativeContext))
	{
		UE_LOG(LogTemp, Log, TEXT("StartGraph: EntryNode %s is not activable in context."), *Graph->GetRootNode()->GetName());
		return;
	}

	if (ActiveGraphs.Contains(Graph))
	{
		UE_LOG(LogTemp, Warning, TEXT("StartGraph: Graph %s is already running."), *Graph->GetName());
		return;
	}

	ActiveGraphs.Add(Graph);
	
	Graph->GetRootNode()->NativeExecute(NarrativeContext, this);
}

bool USSNarrativeManager::IsGraphActive(USSNarrativeGraph* Graph) const
{
	return ActiveGraphs.Contains(Graph);
}

void USSNarrativeManager::ContinueFromNode(USSNarrativeNode* FinishedNode)
{
	if (!FinishedNode || !NarrativeContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("ContinueFromNode: FinishedNode or NarrativeContext is null."));
		return;
	}
	
	// Go through the next nodes
	for (const FSSNarrativeNodeLink& Link : FinishedNode->OutgoingLinks)
	{
		if (Link.TargetNode && Link.TargetNode->CanActivate(NarrativeContext))
		{
			Link.TargetNode->NativeExecute(NarrativeContext, this);
			return;
		}
	}

	// Aucun node activable → le graphe est terminé
	if (USSNarrativeGraph* OwningGraph = Cast<USSNarrativeGraph>(FinishedNode->GetOuter()))
	{
		if (ActiveGraphs.Remove(OwningGraph) > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Narrative graph %s finished (no more valid next nodes)."), *OwningGraph->GetName());
		}
	}
}
