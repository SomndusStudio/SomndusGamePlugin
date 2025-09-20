/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Narrative/AssetTypeActions_SSNarrativeGraph.h"

#include "Narrative/SSNarrativeGraphEditor.h"

void FAssetTypeActions_SSNarrativeGraph::GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section)
{
	FAssetTypeActions_Base::GetActions(InObjects, Section);
}

TSharedPtr<SWidget> FAssetTypeActions_SSNarrativeGraph::GetThumbnailOverlay(const FAssetData& AssetData) const
{
	return FAssetTypeActions_Base::GetThumbnailOverlay(AssetData);
}

void FAssetTypeActions_SSNarrativeGraph::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for (UObject* Object : InObjects)
	{
		if (USSNarrativeGraph* NarrativeGraph = Cast<USSNarrativeGraph>(Object))
		{
			TSharedRef<FSSNarrativeGraphEditor> Editor = MakeShareable(
				new FSSNarrativeGraphEditor());
			Editor->Init(NarrativeGraph, EToolkitMode::Standalone, EditWithinLevelEditor);
		}
	}
}
