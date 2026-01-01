/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/Context/SSInputContextDefinition.h"

#include "Core/SSGameSubsystem.h"
#include "Input/Context/SSInputHandlerContext_Default.h"

USSInputContextDefinition::USSInputContextDefinition()
{
	InputHandlerContextClass = USSInputHandlerContext_Default::StaticClass();
}

#if WITH_EDITORONLY_DATA
void USSInputContextDefinition::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	AddAdditionalAssetBundleData();
}

void USSInputContextDefinition::AddAdditionalAssetBundleData()
{
	AssetBundleData.AddBundleAsset(USSGameSubsystem::LoadStateGameCore, InputHandlerContextClass.ToSoftObjectPath().GetAssetPath());

	for (const auto& Entry : InputContextSettings)
	{
		AssetBundleData.AddBundleAsset(USSGameSubsystem::LoadStateGameCore, Entry.InputAction.ToSoftObjectPath().GetAssetPath());
		AssetBundleData.AddBundleAsset(USSGameSubsystem::LoadStateGameCore, Entry.InputActionHandlerClass.ToSoftObjectPath().GetAssetPath());
	}
}
#endif