// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "SSNarrativeNode.h"
#include "SSNarrativeTypes.h"
#include "SSNarrativeGraph.generated.h"

// Forward-declare ton delegate signature (si tu veux l'étendre ensuite)
DECLARE_MULTICAST_DELEGATE(FOnNarrativeGraphChanged);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SOMNDUSGAME_API USSNarrativeGraph : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Narrative")
	FSSContextBinding ContextBinding;

	/** All node instances stored within this graph */
	UPROPERTY(EditAnywhere, Instanced, Category="Narrative", meta=(HideInDetailPanel))
	TArray<USSNarrativeNode*> AllNodes;

	USSNarrativeNode* GetRootNode() const;
	
	bool IsValidGraph() const;

#if WITH_EDITOR
	USSNarrativeNode* AddNarrativeNode(TSubclassOf<USSNarrativeNode> NodeClass);

	bool RemoveNode(USSNarrativeNode* Node);
	
	FOnNarrativeGraphChanged OnGraphChanged;
	
	void MarkGraphDirty()
	{
		MarkPackageDirty();
		OnGraphChanged.Broadcast();
	}

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
