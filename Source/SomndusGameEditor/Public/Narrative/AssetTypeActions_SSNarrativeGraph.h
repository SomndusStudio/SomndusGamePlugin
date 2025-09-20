/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "SomndusGameEditor.h"
#include "Narrative/SSNarrativeGraph.h"
#include "UObject/Object.h"


class SOMNDUSGAMEEDITOR_API FAssetTypeActions_SSNarrativeGraph : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "SSNarrativeGraph", "Narrative Graph"); }
	virtual FColor GetTypeColor() const override { return FColor(102, 25, 127); }
	virtual UClass* GetSupportedClass() const override { return USSNarrativeGraph::StaticClass(); }
	virtual uint32 GetCategories() override { return FSomndusGameEditorModule::GetSSAssetsCategory(); }

	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
	virtual void GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section) override;
	virtual TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
};