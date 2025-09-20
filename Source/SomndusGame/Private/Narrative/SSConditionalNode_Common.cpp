// Fill out your copyright notice in the Description page of Project Settings.


#include "Narrative/SSConditionalNode_Common.h"

#include "Narrative/SSNarrativeContext.h"

bool USSConditionalNode_Common::CanActivate_Implementation(USSNarrativeContext* Context) const
{
	if (!Context)
		return false;

	const TArray<FName>* FlagsPtr = nullptr;
	const FGameplayTagContainer* TagsPtr = nullptr;

	if (TargetProfile.IsNone())
	{
		FlagsPtr = &Context->Flags;
		TagsPtr = &Context->ActiveNarrativeTags;
	}
	else if (const FSSNarrativeProfile* Profile = Context->GetProfile(TargetProfile))
	{
		FlagsPtr = &Profile->Flags;
		TagsPtr = &Profile->Tags;
	}
	else
	{
		return false;
	}

	switch (ConditionType)
	{
	case ESSConditionType::HasFlag:
		return FlagsPtr->Contains(Flag);

	case ESSConditionType::MissingFlag:
		return !FlagsPtr->Contains(Flag);

	case ESSConditionType::HasGameplayTag:
		return TagsPtr->HasTag(GameplayTag);

	case ESSConditionType::MissingGameplayTag:
		return !TagsPtr->HasTag(GameplayTag);

	default:
		return false;
	}
}
