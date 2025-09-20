// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSNarrativeNode.h"

#include "Narrative/SSNarrativeManager.h"

bool USSNarrativeNode::CanActivate_Implementation(class USSNarrativeContext* Context) const
{
	return true;
}

void USSNarrativeNode::Execute_Implementation(USSNarrativeContext* Context, class USSNarrativeManager* InManager)
{
	ExecuteFinished();
}

void USSNarrativeNode::NativeExecute(USSNarrativeContext* Context, class USSNarrativeManager* InManager)
{
	Manager = InManager;

	Execute(Context, Manager);
}

void USSNarrativeNode::ExecuteFinished()
{
	// Callback une fois le dialogue terminé
	Manager->ContinueFromNode(this);
}

FText USSNarrativeNode::GetDisplayName_Implementation() const
{
	return FText::FromString(GetClass()->GetName().Replace(TEXT("USSNarrativeNode_"), TEXT("")));
}

class UWorld* USSNarrativeNode::GetWorld() const
{
	if (Manager)
	{
		return Manager->GetWorld();
	}
	return Super::GetWorld();
}
