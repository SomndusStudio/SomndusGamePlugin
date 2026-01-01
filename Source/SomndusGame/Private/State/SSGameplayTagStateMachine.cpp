/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "State/SSGameplayTagStateMachine.h"

USSGameplayTagStateMachine::USSGameplayTagStateMachine()
{
	// Par défaut, pas d'état
	CurrentState = FGameplayTag::EmptyTag;
}

bool USSGameplayTagStateMachine::TrySetState(FGameplayTag NewState)
{
	if (CanTransitionTo(NewState))
	{
		FGameplayTag Previous = CurrentState;
		CurrentState = NewState;
		OnStateChanged.Broadcast(Previous, NewState);
		return true;
	}

	return false;
}

void USSGameplayTagStateMachine::ForceSetState(FGameplayTag NewState)
{
	FGameplayTag Previous = CurrentState;
	CurrentState = NewState;
	OnStateChanged.Broadcast(Previous, NewState);
}

bool USSGameplayTagStateMachine::CanTransitionTo(FGameplayTag NewState) const
{
	if (!AllowedStates.IsEmpty() && !AllowedStates.HasTagExact(NewState))
	{
		return false;
	}

	if (!ValidTransitions.Contains(CurrentState))
	{
		return false;
	}

	const FGameplayTagContainer& Allowed = ValidTransitions[CurrentState];
	return Allowed.HasTagExact(NewState);
}

FGameplayTag USSGameplayTagStateMachine::GetCurrentState() const
{
	return CurrentState;
}

bool USSGameplayTagStateMachine::HasState(FGameplayTag InState)
{
	return CurrentState == InState;
}
