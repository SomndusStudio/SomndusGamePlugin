/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "SSNarrativeGraphEditorComponents.h"
#include "Misc/NotifyHook.h"
#include "Narrative/SSNarrativeGraph.h"
#include "Toolkits/AssetEditorToolkit.h"


/**
 * Customizes the details panel layout for USSNarrativeGraph assets.
 */
class FSSNarrativeGraphCustomization : public IDetailCustomization
{
public:
	/** Creates a new instance of this detail customization */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};

/**
 * Asset editor for USSNarrativeGraph assets.
 * Provides graph editing, node manipulation, and compile status handling.
 */
class FSSNarrativeGraphEditor : public FAssetEditorToolkit, public FNotifyHook
{
public:
	FSSNarrativeGraphEditor();
	virtual ~FSSNarrativeGraphEditor();

	/**
	 * Initializes the narrative graph editor.
	 * @param InAsset - The graph asset to edit.
	 * @param Mode - How the editor is launched (standalone, world-centric).
	 * @param InitToolkitHost - Optional host for world-centric editing.
	 */
	void Init(USSNarrativeGraph* InAsset, const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost);

	/** Called when the graph asset is modified */
	void OnGraphAssetModified();

	/** Handles Compile button click */
	void HandleCompile();

	/** Synchronizes visual graph nodes to the runtime narrative nodes */
	void SynchronizeEditorNodesToRuntime();

	/** Rebuilds runtime narrative node links based on editor graph */
	void RebuildRuntimeLinksFromEditor();

	/** Creates a new node from the given class at the specified position */
	USSNarrativeNode* CreateNode(TSubclassOf<USSNarrativeNode> NodeClass, UEdGraphPin* FromPin, FVector2D Position);

	/** Spawns an editor node widget from a runtime node */
	void CreateEditorNodeFromNarrativeNode(USSNarrativeNode* RuntimeNode, UEdGraphPin* FromPin, FVector2D GraphPosition);

	/** Adds a node to the graph from a context menu action */
	void HandleAddNodeFromContextMenu(TSubclassOf<USSNarrativeNode> NodeClass, UEdGraphPin* FromPin, FVector2D GraphPosition);

	/** FAssetEditorToolkit interface */
	virtual void SaveAsset_Execute() override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	/** Tab registration */
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

private:
	/** Initializes the toolbar with custom actions */
	void ExtendToolbar();

	/** Fills the custom toolbar with buttons (e.g., Compile) */
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);

	/** Binds graph editor commands */
	void BindGraphCommands();

	/** Refreshes the graph editor view */
	void RefreshGraphEditor();

	/** Called when selection changes in the graph editor */
	void OnGraphSelectionChanged(const TSet<UObject*>& NewSelection);

	/** Called when details are modified in the node details view */
	void OnNodeDetailsChanged(const FPropertyChangedEvent& InEvent);

	/** Deletes currently selected nodes */
	void DeleteSelectedNodes();

	/** Whether the current graph selection allows node deletion */
	bool CanDeleteNodes() const;

	/** Returns the compile status icon for the toolbar */
	FSlateIcon GetCompileStatusIcon() const;

	/** Returns the compile status tooltip for the toolbar */
	FText GetCompileStatusTooltip() const;

	/** Spawns the Details tab */
	TSharedRef<SDockTab> SpawnDetailsTab(const FSpawnTabArgs& Args);

	/** Spawns the Graph Editor tab */
	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args);

private:
	/** The graph asset being edited */
	USSNarrativeGraph* Asset = nullptr;

	/** Cached pointer to the editor version of the graph */
	USSNarrativeGraph_EdGraph* EdNarrativeGraph = nullptr;

	/** Main graph editor widget */
	TSharedPtr<SGraphEditor> GraphEditor;

	/** Details views for the graph and selected nodes */
	TSharedPtr<IDetailsView> GraphDetailsView;
	TSharedPtr<IDetailsView> NodeDetailsView;

	/** Command list for graph-specific actions */
	TSharedPtr<FUICommandList> GraphEditorCommands;

	/** Command list for general toolbar actions */
	TSharedPtr<FUICommandList> ToolbarCommands;

	/** Current compile status of the graph */
	EBlueprintStatus CompileStatus = EBlueprintStatus::BS_Unknown;

	/** Notifies the editor that the asset is dirty */
	void NotifyPackageDirty();

	/** Creates a visual node widget for the specified runtime node */
	USSNarrativeGraphEditorGraphNode* CreateVisualNode(USSNarrativeNode* RuntimeNode);

	/** Tab IDs */
	static const FName DetailsTabID;
	static const FName GraphTabID;
};
