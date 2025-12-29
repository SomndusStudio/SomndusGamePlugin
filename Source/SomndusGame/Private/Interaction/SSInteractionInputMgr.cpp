// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/SSInteractionInputMgr.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Interaction/SSInteractable.h"


void USSInteractionInputMgr::OnInteractionSuspendDelayEnd()
{
	bInteractionIsActive = false;
}

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
					if (GetWorld())
					{
						bInteractionIsActive = true;
						GetWorld()->GetTimerManager().ClearTimer(InteractionSuspendDelay);
						GetWorld()->GetTimerManager().SetTimer(InteractionSuspendDelay, this, &ThisClass::OnInteractionSuspendDelayEnd, 0.1f, false);
					}
					ISSInteractable::Execute_OnInteract(AwaitInteractionContext.Object, AwaitInteractionContext);
				}
			}
			return true;
		}
	}
	return false;
}

bool USSInteractionInputMgr::IsInteractionIsActive() const
{
	return bInteractionIsActive;
}

