// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/SSHelperStatics.h"

TArray<FName> USSHelperStatics::ConvertGTagsToFTags(const TArray<FGameplayTag>& InGameplayTags)
{
	TArray<FName> Results;

	for(auto const& Tags : InGameplayTags)
	{
		Results.Add(Tags.GetTagName());
	}
	return Results;
}
