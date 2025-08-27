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

bool USSHelperStatics::CallFunctionByName(UObject* Target, const FString& FunctionName)
{
	if (!Target)
	{
		return false;
	}

	UFunction* Function = Target->FindFunction(FName(*FunctionName));
	if (Function)
	{
		Target->ProcessEvent(Function, nullptr);
		return true;
	}

	return false;
}

UObject* USSHelperStatics::BP_TryGetAsset(const TSoftObjectPtr<UObject>& SoftObject, const FString& Context)
{
	return 	TryGetAsset(SoftObject, Context);
}

UClass* USSHelperStatics::BP_TryGetClass(const TSoftClassPtr<UObject>& SoftClass, const FString& Context)
{
	return TryGetClass(SoftClass, Context);
}
