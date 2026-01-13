/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSGameInputData.h"

#include "Core/SSGameSubsystem.h"

UInputAction* USSInputActionIdRegistry::GetInputAction(int32 InputId)
{
	if (const auto* InputActionRef = InputActionIdMap.Find(InputId))
	{
		return *InputActionRef;
	}
	return nullptr;
}


////////////////////////////////////////////////////////
// USSInputMappingGameModeAsset
#if WITH_EDITORONLY_DATA
void USSInputMappingGameModeAsset::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (const auto& MappingContext : MappingContexts)
	{
		AssetBundleData.AddBundleAsset(USSGameSubsystem::LoadStateGameCore, MappingContext.ToSoftObjectPath().GetAssetPath());
	}
}
#endif
