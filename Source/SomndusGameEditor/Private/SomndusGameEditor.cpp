#include "SomndusGameEditor.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
#include "SSStyle.h"
#include "Core/SSEditorCommands.h"
#include "Narrative/AssetTypeActions_SSNarrativeGraph.h"
#include "Narrative/SSNarrativeGraphEditorComponents.h"
#include "Narrative/SSNarrativeGraphEditor.h"

#define LOCTEXT_NAMESPACE "FSomndusGameEditorModule"

TSharedPtr<IAssetTypeActions> SSNarrativeActions;

EAssetTypeCategories::Type FSomndusGameEditorModule::SSCategoryBit;

////////////////////////////////////////////////////////////////////
// Module

void FSomndusGameEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// SS Category
	SSCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("SS")), LOCTEXT("SSAssetsCategory", "Somndus Studio"));

	// IDetailCustomization
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(
		"SSNarrativeGraph",
		FOnGetDetailCustomizationInstance::CreateStatic(&FSSNarrativeGraphCustomization::MakeInstance)
	);

	RegisterAssetActions();

	//Register the commands
	FSSEditorCommands::Register();

	FSSStyle::Initialize();
}

void FSomndusGameEditorModule::ShutdownModule()
{
	FSSEditorCommands::Unregister();

	FSSStyle::Shutdown();
	
	if (NarrativeGraphNodeFactor.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(NarrativeGraphNodeFactor.ToSharedRef());
		NarrativeGraphNodeFactor.Reset();
	}

	// IDetailCustomization
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout("RPGNarrativeGraph");
}

EAssetTypeCategories::Type FSomndusGameEditorModule::GetSSAssetsCategory()
{
	return SSCategoryBit;
}

void FSomndusGameEditorModule::RegisterAssetActions()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	// Initialize and register the visual node factory
	NarrativeGraphNodeFactor = MakeShared<FSSNarrativeGraphNodeFactory>();
	FEdGraphUtilities::RegisterVisualNodeFactory(NarrativeGraphNodeFactor);

	// Register asset type actions (for context menus, asset editor etc.)
	SSNarrativeActions = MakeShareable(new FAssetTypeActions_SSNarrativeGraph);
	AssetTools.RegisterAssetTypeActions(SSNarrativeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSomndusGameEditorModule, SomndusGameEditor)