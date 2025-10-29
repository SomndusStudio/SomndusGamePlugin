// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/SSInteractionInputMgr.h"

#include "Interaction/SSInteractable.h"


bool USSInteractionInputMgr::PushInteractionContext(const FSSInteractionContext& InteractionContext)
{
	// Replace if higher priority
	const FSSInteractionContext* FoundInteraction = AwaitInteractionContexts.FindByPredicate([InteractionContext](const FSSInteractionContext& Value)
	{
		return InteractionContext.Equals(Value);
	});
	if (FoundInteraction)
	{
		// Remove old before replace
		if (InteractionContext.Prompt.Priority <= FoundInteraction->Prompt.Priority)
		{
			RemoveInteractionContext(*FoundInteraction);
		}
		else
		{
			return false;
		}
	}
	AwaitInteractionContexts.Add(InteractionContext);
	OnInteractionContextPush.Broadcast(this, InteractionContext);
	return true;
}


bool USSInteractionInputMgr::RemoveInteractionContext(const FSSInteractionContext& InteractionContext)
{
	// Remove await
	AwaitInteractionContexts.RemoveAll([InteractionContext](const FSSInteractionContext& Value)
	{
		return InteractionContext.Equals(Value);
	});
	// Notify
	OnInteractionContextRemove.Broadcast(this, InteractionContext);
	return false;
}

bool USSInteractionInputMgr::CheckInputInteraction(const UInputAction* EnhancedInputAction)
{
	for (const auto& AwaitInteractionContext: AwaitInteractionContexts)
	{
		if (AwaitInteractionContext.Prompt.EnhancedInputAction == EnhancedInputAction)
		{
			if (IsValid(AwaitInteractionContext.Object))
			{
				if (AwaitInteractionContext.Object->Implements<USSInteractable>())
				{
					ISSInteractable::Execute_OnInteract(AwaitInteractionContext.Object, AwaitInteractionContext);
				}
			}
			return true;
		}
	}
	return false;
}

