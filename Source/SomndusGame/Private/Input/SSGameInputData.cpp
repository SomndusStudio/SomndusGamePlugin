// ©2025 Schartier Isaac. All rights reserved.


#include "Input/SSGameInputData.h"

#include "Core/SSGameSubsystem.h"

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
