/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


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

int USSHelperStatics::GetPlayInEditorID()
{
	return GPlayInEditorID;
}
