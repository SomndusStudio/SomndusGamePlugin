// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSNarrativeGraph.h"
#include "SSContextObject.h"
#include "SSNarrativeManager.generated.h"

class SSNarrativeSubsystem;

/**
 * 
 */
UCLASS(Blueprintable)
class SOMNDUSGAME_API USSNarrativeManager : public USSContextObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	USSNarrativeContext* GlobalContext;
	
	UPROPERTY(BlueprintReadWrite)
	USSNarrativeContext* NarrativeContext;

	/** Keeps track of active graphs currently executing */
	UPROPERTY()
	TSet<USSNarrativeGraph*> ActiveGraphs;
	
	/**
	 * Returns the global narrative context shared by the game.
	 */
	USSNarrativeContext* GetGlobalContext();

	void SetGlobalContext(USSNarrativeContext* GlobalContext);
	
	/**
	 * Creates a narrative context that includes only the specified profile.
	 * This context is isolated and temporary unless stored.
	 */
	USSNarrativeContext* CreateContextFromProfileId(FName ProfileId);
	
	USSNarrativeContext* ResolveContext(const FSSContextBinding& Binding, UObject* RuntimeSource);
	/**
	  * Starts a narrative graph from its entry node.
	  */
	UFUNCTION(BlueprintCallable)
	void StartGraph(USSNarrativeGraph* Graph, UObject* RuntimeSource);

	/** Returns whether the specified graph is currently running */
	UFUNCTION(BlueprintCallable, Category="Narrative")
	bool IsGraphActive(USSNarrativeGraph* Graph) const;
	
	/**
	 * Called by a node when its execution is complete.
	 * It will continue narrative flow using the NextNodes.
	 */
	UFUNCTION(BlueprintCallable)
	void ContinueFromNode(USSNarrativeNode* FinishedNode);
};
