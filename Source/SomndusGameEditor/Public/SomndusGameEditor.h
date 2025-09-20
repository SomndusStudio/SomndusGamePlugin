/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"
#include "Narrative/SSNarrativeGraphEditorComponents.h"

class FSomndusGameEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static EAssetTypeCategories::Type GetSSAssetsCategory();
    
private:
    
    void RegisterAssetActions();
    
public:
    TSharedPtr<FSSNarrativeGraphNodeFactory> NarrativeGraphNodeFactor;

    static EAssetTypeCategories::Type SSCategoryBit;
};
