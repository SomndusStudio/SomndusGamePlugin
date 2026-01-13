// ©2025 Schartier Isaac. All rights reserved.

#include "Mode/SSGameModeActionSet.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "DUGameModeActionSet"

USSGameModeActionSet::USSGameModeActionSet()
{
}

#if WITH_EDITORONLY_DATA
EDataValidationResult USSGameModeActionSet::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			EDataValidationResult ChildResult = Action->IsDataValid(Context);
			Result = CombineDataValidationResults(Result, ChildResult);
		}
		else
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("ActionEntryIsNull", "Null entry at index {0} in Actions"), FText::AsNumber(EntryIndex)));
		}

		++EntryIndex;
	}

	return Result;
}
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITORONLY_DATA
void USSGameModeActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();
	
	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif // WITH_EDITORONLY_DATA

#undef LOCTEXT_NAMESPACE