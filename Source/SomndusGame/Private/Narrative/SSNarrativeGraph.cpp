// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSNarrativeGraph.h"

USSNarrativeNode* USSNarrativeGraph::GetRootNode() const
{
	if (AllNodes.Num() == 0) return nullptr;
	return AllNodes[0];
}

bool USSNarrativeGraph::IsValidGraph() const
{
	return IsValid(GetRootNode());
}

#if WITH_EDITOR
USSNarrativeNode* USSNarrativeGraph::AddNarrativeNode(TSubclassOf<USSNarrativeNode> NodeClass)
{
	if (!NodeClass) return nullptr;

	USSNarrativeNode* NewNode = NewObject<USSNarrativeNode>(this, NodeClass);
	AllNodes.Add(NewNode);
	
	MarkGraphDirty();
	
	return NewNode;
}

bool USSNarrativeGraph::RemoveNode(USSNarrativeNode* Node)
{
	AllNodes.Remove(Node);
	Modify();

	MarkGraphDirty();

	return true;
}

void USSNarrativeGraph::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	OnGraphChanged.Broadcast();
}

#endif