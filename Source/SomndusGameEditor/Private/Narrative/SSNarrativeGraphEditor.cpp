/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Narrative/SSNarrativeGraphEditor.h"

#include "DetailLayoutBuilder.h"
#include "Core/SSEditorLog.h"
#include "Core/SSEditorCommands.h"
#include "Framework/Commands/GenericCommands.h"
#include "Narrative/SSNarrativeGraphEditorComponents.h"
#include "Widgets/Docking/SDockTab.h"

const FName FSSNarrativeGraphEditor::DetailsTabID(TEXT("SSNarrativeGraphEditor_Details"));
const FName FSSNarrativeGraphEditor::GraphTabID(TEXT("SSNarrativeGraphEditor_Graph"));

#define LOCTEXT_NAMESPACE "FSomndusGameEditorModule"

/////////////////////////////////////////////////////
// FSSNarrativeGraphCustomization

/** Factory method to instantiate the detail customization */
TSharedRef<IDetailCustomization> FSSNarrativeGraphCustomization::MakeInstance()
{
	return MakeShareable(new FSSNarrativeGraphCustomization);
}

/** Customizes the layout of the Narrative Graph details panel */
void FSSNarrativeGraphCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Hide AllNodes from the property panel (redundant with graph)
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(USSNarrativeGraph, AllNodes));
}

/////////////////////////////////////////////////////
// FSSNarrativeGraphEditor

FSSNarrativeGraphEditor::FSSNarrativeGraphEditor()
{
}

FSSNarrativeGraphEditor::~FSSNarrativeGraphEditor()
{
	if (Asset)
	{
		Asset->OnGraphChanged.RemoveAll(this);
	}
}

void FSSNarrativeGraphEditor::Init(USSNarrativeGraph* InAsset, const EToolkitMode::Type Mode,
                                    const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	Asset = InAsset;
	Asset->OnGraphChanged.AddRaw(this, &FSSNarrativeGraphEditor::OnGraphAssetModified);

	// Get default compile status
	if (Asset->GetPackage()->IsDirty())
	{
		CompileStatus = EBlueprintStatus::BS_Dirty;
	}
	else
	{
		CompileStatus = EBlueprintStatus::BS_UpToDate;
	}

	// Commands
	GraphEditorCommands = MakeShareable(new FUICommandList);
	BindGraphCommands();

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("SSNarrativeGraphEditorLayout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split(FTabManager::NewSplitter()
			        ->SetOrientation(Orient_Horizontal)
			        ->SetSizeCoefficient(0.9f)
			        ->Split
			        (
				        FTabManager::NewStack()
				        ->SetSizeCoefficient(0.35f)
				        ->AddTab(DetailsTabID, ETabState::OpenedTab)
			        )
			        ->Split(
				        FTabManager::NewStack()
				        ->SetSizeCoefficient(0.65f)
				        ->AddTab(GraphTabID, ETabState::OpenedTab)
			        )
			)
		);

	//Init the asset editor window
	InitAssetEditor(Mode, InitToolkitHost,
	                FName("SSNarrativeGraphEditorApp"),
	                Layout,
	                true, true, InAsset);

	//Extensions ~ Toolbar
	ExtendToolbar();
	RegenerateMenusAndToolbars();
}

void FSSNarrativeGraphEditor::OnGraphAssetModified()
{
	if (CompileStatus != BS_Dirty)
	{
		CompileStatus = BS_Dirty;
	}
}

void FSSNarrativeGraphEditor::HandleCompile()
{
	// Sync editor graph node positions back into logical nodes
	SynchronizeEditorNodesToRuntime();
	RebuildRuntimeLinksFromEditor();

	if (Asset)
	{
		// Register undo for asset before save
		Asset->Modify();
	}

	if (Asset->IsValidGraph())
	{
		CompileStatus = BS_UpToDate;
	}
	else
	{
		CompileStatus = BS_Error;
	}
}

void FSSNarrativeGraphEditor::SynchronizeEditorNodesToRuntime()
{
	if (!Asset)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error, TEXT("[NarrativeGraph] Cannot synchronize: Asset is null."));
		return;
	}

	if (!EdNarrativeGraph)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error,
		       TEXT("[NarrativeGraph] Cannot synchronize: EdNarrativeGraph is null."));
		return;
	}

	for (UEdGraphNode* GraphNode : EdNarrativeGraph->Nodes)
	{
		USSNarrativeGraphEditorGraphNode* VisualNode = Cast<USSNarrativeGraphEditorGraphNode>(GraphNode);
		if (!VisualNode)
		{
			UE_LOG(LogSSNarrativeGraphEditor, Warning,
			       TEXT("[NarrativeGraph] Skipping node: not a valid USSNarrativeGraphEditorGraphNode."));
			continue;
		}

		if (!VisualNode->SourceNode)
		{
			UE_LOG(LogSSNarrativeGraphEditor, Warning,
			       TEXT("[NarrativeGraph] Editor node '%s' has no source runtime node. Skipping."),
			       *VisualNode->GetName());
			continue;
		}

		// Sync position from visual node to runtime node
		VisualNode->SourceNode->EditorPosition = FVector2D(VisualNode->NodePosX, VisualNode->NodePosY);
	}

	UE_LOG(LogSSNarrativeGraphEditor, Log, TEXT("[NarrativeGraph] Editor node positions synchronized to runtime."));
}


void FSSNarrativeGraphEditor::RebuildRuntimeLinksFromEditor()
{
	if (!Asset || !EdNarrativeGraph)
	{
		return;
	}

	// ────────────────────────────────
	// Clear all runtime links
	// ────────────────────────────────
	for (USSNarrativeNode* RuntimeNode : Asset->AllNodes)
	{
		if (RuntimeNode)
		{
			RuntimeNode->OutgoingLinks.Empty();
		}
	}

	// ────────────────────────────────
	// Rebuild links from graph editor
	// ────────────────────────────────
	for (UEdGraphNode* EditorNode : EdNarrativeGraph->Nodes)
	{
		USSNarrativeGraphEditorGraphNode* SourceEdNode = Cast<USSNarrativeGraphEditorGraphNode>(EditorNode);
		if (!SourceEdNode || !SourceEdNode->SourceNode)
		{
			continue;
		}

		for (UEdGraphPin* OutputPin : SourceEdNode->Pins)
		{
			if (!OutputPin || OutputPin->Direction != EGPD_Output)
			{
				continue;
			}

			for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
			{
				if (!LinkedPin || !LinkedPin->GetOwningNode())
				{
					continue;
				}

				USSNarrativeGraphEditorGraphNode* TargetEdNode =
					Cast<USSNarrativeGraphEditorGraphNode>(LinkedPin->GetOwningNode());

				if (!TargetEdNode || !TargetEdNode->SourceNode)
				{
					continue;
				}

				// Build and add the runtime link
				FSSNarrativeNodeLink Link;
				Link.TargetNode = TargetEdNode->SourceNode;
				Link.LinkLabel = FName(OutputPin->PinName.ToString());

				SourceEdNode->SourceNode->OutgoingLinks.Add(Link);
			}
		}
	}
}

USSNarrativeNode* FSSNarrativeGraphEditor::CreateNode(TSubclassOf<USSNarrativeNode> NodeClass, UEdGraphPin* FromPin, FVector2D Position)
{
	if (!Asset)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error, TEXT("[NarrativeGraph] CreateNode failed: Asset is null."));
		return nullptr;
	}

	if (!EdNarrativeGraph)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error,
		       TEXT("[NarrativeGraph] CreateNode failed: EdNarrativeGraph is null."));
		return nullptr;
	}

	if (!NodeClass)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Warning, TEXT("[NarrativeGraph] CreateNode failed: NodeClass is null."));
		return nullptr;
	}

	// Create runtime node
	USSNarrativeNode* RuntimeNode = Asset->AddNarrativeNode(NodeClass);
	if (!RuntimeNode)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error, TEXT("[NarrativeGraph] Failed to create runtime node of class '%s'."),
		       *NodeClass->GetName());
		return nullptr;
	}

	RuntimeNode->EditorPosition = Position;

	// Create editor visual node
	USSNarrativeGraphEditorGraphNode* EdNode = CreateVisualNode(RuntimeNode);
	if (!EdNode)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Warning,
		       TEXT("[NarrativeGraph] Visual node creation failed for runtime node '%s'."), *RuntimeNode->GetName());
	}

	// Autowire
	if (FromPin)
	{
		EdNode->AutowireNewNode(FromPin);
	}
	
	UE_LOG(LogSSNarrativeGraphEditor, Log, TEXT("[NarrativeGraph] Node '%s' created at (%.1f, %.1f)."),
	       *NodeClass->GetName(), Position.X, Position.Y);

	return RuntimeNode;
}

void FSSNarrativeGraphEditor::CreateEditorNodeFromNarrativeNode(USSNarrativeNode* RuntimeNode, UEdGraphPin* FromPin,
                                                                 FVector2D GraphPosition)
{
	if (!RuntimeNode)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Warning,
		       TEXT("[NarrativeGraph] Tried to create editor node from null RuntimeNode."));
		return;
	}

	if (!Asset)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error,
		       TEXT("[NarrativeGraph] Asset is null in CreateEditorNodeFromNarrativeNode."));
		return;
	}

	if (!EdNarrativeGraph)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error,
		       TEXT("[NarrativeGraph] EdNarrativeGraph is null in CreateEditorNodeFromNarrativeNode."));
		return;
	}

	// Create visual node
	CreateNode(RuntimeNode->GetClass(), FromPin, GraphPosition);

	UE_LOG(LogSSNarrativeGraphEditor, Log,
	       TEXT("[NarrativeGraph] Editor node created for '%s' at position (%.1f, %.1f)."),
	       *RuntimeNode->GetClass()->GetName(), GraphPosition.X, GraphPosition.Y);
}

void FSSNarrativeGraphEditor::HandleAddNodeFromContextMenu(TSubclassOf<USSNarrativeNode> NodeClass, UEdGraphPin* FromPin,
                                                            FVector2D GraphPosition)
{
	if (!NodeClass)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Warning, TEXT("[NarrativeGraph] Invalid node class provided."));
		return;
	}

	if (!GraphEditor.IsValid())
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error, TEXT("[NarrativeGraph] GraphEditor is not valid."));
		return;
	}

	USSNarrativeNode* Node = CreateNode(NodeClass, FromPin, GraphPosition);
	if (!Node)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error, TEXT("[NarrativeGraph] Failed to create node of class '%s'."),
		       *NodeClass->GetName());
		return;
	}
	
	// Select and focus the new node in the editor
	for (UEdGraphNode* EdNode : EdNarrativeGraph->Nodes)
	{
		USSNarrativeGraphEditorGraphNode* TypedNode = Cast<USSNarrativeGraphEditorGraphNode>(EdNode);
		if (!TypedNode || !TypedNode->SourceNode)
		{
			continue;
		}

		if (TypedNode->SourceNode == Node)
		{
			GraphEditor->ClearSelectionSet();
			GraphEditor->SetNodeSelection(TypedNode, true);
			GraphEditor->JumpToNode(TypedNode);

			UE_LOG(LogSSNarrativeGraphEditor, Log,
			       TEXT("[NarrativeGraph] Node of class '%s' added and selected at position (%.1f, %.1f)."),
			       *NodeClass->GetName(), GraphPosition.X, GraphPosition.Y);
			break;
		}
	}
}


/////////////////////////////////////////////////////
// FAssetEditorToolkit interface

void FSSNarrativeGraphEditor::SaveAsset_Execute()
{
	// Compile before saving if the graph has been modified
	if (CompileStatus != EBlueprintStatus::BS_UpToDate)
	{
		HandleCompile();
	}

	// Let the base editor handle the actual asset save
	FAssetEditorToolkit::SaveAsset_Execute();
}

FName FSSNarrativeGraphEditor::GetToolkitFName() const
{
	return FName("SSNarrativeGraphEditor");
}

FText FSSNarrativeGraphEditor::GetBaseToolkitName() const
{
	return NSLOCTEXT("RPGEditor", "SSNarrativeGraphEditor", "Narrative Graph Editor");
}

FString FSSNarrativeGraphEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Narrative Graph Editor");
}

FLinearColor FSSNarrativeGraphEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.4f, 0.1f, 0.5f);
}

/////////////////////////////////////////////////////
// Tab registration
void FSSNarrativeGraphEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(DetailsTabID,
	                                 FOnSpawnTab::CreateRaw(
		                                 this, &FSSNarrativeGraphEditor::SpawnDetailsTab))
	            .SetDisplayName(NSLOCTEXT("RPGEditor", "DetailsTab", "Details"))
	            .SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(GraphTabID,
	                                 FOnSpawnTab::CreateRaw(this, &FSSNarrativeGraphEditor::SpawnGraphTab))
	            .SetDisplayName(NSLOCTEXT("RPGEditor", "GraphTab", "Graph"))
	            .SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FSSNarrativeGraphEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(DetailsTabID);
}

TSharedRef<SDockTab> FSSNarrativeGraphEditor::SpawnDetailsTab(const FSpawnTabArgs& Args)
{
	// Load the Property Editor module
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
		"PropertyEditor");

	// Common settings for both views
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.NotifyHook = this;

	// Main asset details view
	GraphDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	GraphDetailsView->SetObject(Asset);

	// Selected node details view
	NodeDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	NodeDetailsView->OnFinishedChangingProperties().AddSP(this, &FSSNarrativeGraphEditor::OnNodeDetailsChanged);

	return SNew(SDockTab)
		.Label(NSLOCTEXT("RPGEditor", "DetailsTabLabel", "Details"))
		.TabRole(ETabRole::PanelTab)
		[
			SNew(SVerticalBox)

			// ───── Graph Asset ─────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(4)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("GraphDetailsHeader", "Graph Asset"))
				.Font(IDetailLayoutBuilder::GetDetailFontBold())
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				GraphDetailsView.ToSharedRef()
			]

			// ───── Selected Node ─────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8, 16, 8, 4)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("NodeDetailsHeader", "Selected Node"))
				.Font(IDetailLayoutBuilder::GetDetailFontBold())
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				NodeDetailsView.ToSharedRef()
			]
		];
}

TSharedRef<SDockTab> FSSNarrativeGraphEditor::SpawnGraphTab(const FSpawnTabArgs& Args)
{
	// ────────────────────────────────
	// Create transient editor graph
	// ────────────────────────────────
	EdNarrativeGraph = NewObject<USSNarrativeGraph_EdGraph>(
		GetTransientPackage(), USSNarrativeGraph_EdGraph::StaticClass());

	EdNarrativeGraph->Schema = USSNarrativeGraphSchema::StaticClass();
	EdNarrativeGraph->OwningToolkit = SharedThis(this);

	if (Asset)
	{
		EdNarrativeGraph->RuntimeGraph = Asset;

		const UEdGraphSchema* Schema = EdNarrativeGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EdNarrativeGraph);

		TMap<USSNarrativeNode*, USSNarrativeGraphEditorGraphNode*> NodeMap;

		// ───── Root Node
		USSNarrativeGraphEditorGraphNode* RootEditorNode =
			Cast<USSNarrativeGraphEditorGraphNode>(EdNarrativeGraph->Nodes[0]);

		if (USSNarrativeNode* RootRuntimeNode = Asset->GetRootNode())
		{
			NodeMap.Add(RootRuntimeNode, RootEditorNode);
		}

		// ───── Other Nodes
		for (USSNarrativeNode* RuntimeNode : Asset->AllNodes)
		{
			if (!RuntimeNode || RuntimeNode == Asset->GetRootNode())
				continue;

			USSNarrativeGraphEditorGraphNode* EdNode = CreateVisualNode(RuntimeNode);
			NodeMap.Add(RuntimeNode, EdNode);
		}

		// ───── Build Pin Links
		for (USSNarrativeNode* SourceRuntimeNode : Asset->AllNodes)
		{
			USSNarrativeGraphEditorGraphNode** SourceEdNodePtr = NodeMap.Find(SourceRuntimeNode);
			if (!SourceEdNodePtr || !*SourceEdNodePtr)
				continue;

			USSNarrativeGraphEditorGraphNode* SourceEdNode = *SourceEdNodePtr;

			for (const FSSNarrativeNodeLink& Link : SourceRuntimeNode->OutgoingLinks)
			{
				if (!Link.TargetNode)
					continue;

				USSNarrativeGraphEditorGraphNode** TargetEdNodePtr = NodeMap.Find(Link.TargetNode);
				if (!TargetEdNodePtr || !*TargetEdNodePtr)
					continue;

				USSNarrativeGraphEditorGraphNode* TargetEdNode = *TargetEdNodePtr;

				if (SourceEdNode->Pins.Num() == 0 || TargetEdNode->Pins.Num() == 0)
					continue;

				UEdGraphPin* OutputPin = SourceEdNode->Pins.Last(); // convention UE: output = last (right)
				UEdGraphPin* InputPin = TargetEdNode->Pins[0]; // convention UE: input = first (left)

				if (OutputPin && InputPin)
				{
					OutputPin->MakeLinkTo(InputPin);
				}
			}
		}
	}

	// ────────────────────────────────
	// Create the GraphEditor Widget
	// ────────────────────────────────
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = NSLOCTEXT("RPGEditor", "GraphCorner", "Narrative Graph");

	SGraphEditor::FGraphEditorEvents Events;
	Events.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(
		this, &FSSNarrativeGraphEditor::OnGraphSelectionChanged);

	GraphEditor = SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EdNarrativeGraph)
		.GraphEvents(Events)
		.IsEditable(true);

	return SNew(SDockTab)
		.Label(NSLOCTEXT("RPGEditor", "GraphTabLabel", "Graph"))
		.TabRole(ETabRole::PanelTab)
		[
			GraphEditor.ToSharedRef()
		];
}

/////////////////////////////////////////////////////
// Toolbar & Command

void FSSNarrativeGraphEditor::ExtendToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateRaw(this, &FSSNarrativeGraphEditor::FillToolbar)
	);

	AddToolbarExtender(ToolbarExtender);
}

void FSSNarrativeGraphEditor::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	const FSSEditorCommands& Commands = FSSEditorCommands::Get();

	ToolbarBuilder.BeginSection("Compile");
	{
		ToolbarBuilder.SetStyle(&FAppStyle::Get(), "CalloutToolbar");

		ToolbarBuilder.AddToolBarButton(
			Commands.Compile,
			NAME_None,
			TAttribute<FText>(),
			TAttribute<FText>::CreateSP(this, &FSSNarrativeGraphEditor::GetCompileStatusTooltip),
			TAttribute<FSlateIcon>::CreateSP(this, &FSSNarrativeGraphEditor::GetCompileStatusIcon)
		);
	}
	ToolbarBuilder.EndSection();
}

void FSSNarrativeGraphEditor::BindGraphCommands()
{
	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FSSNarrativeGraphEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FSSNarrativeGraphEditor::CanDeleteNodes)
	);

	const FSSEditorCommands& Commands = FSSEditorCommands::Get();

	ToolkitCommands->MapAction(
		Commands.Compile,
		FExecuteAction::CreateSP(this, &FSSNarrativeGraphEditor::HandleCompile)
	);
}


void FSSNarrativeGraphEditor::OnGraphSelectionChanged(const TSet<UObject*>& NewSelection)
{
	// Show selected node in the details view if exactly one node is selected
	if (NewSelection.Num() == 1)
	{
		UObject* SelectedObject = *NewSelection.CreateConstIterator();
		UEdGraphNode* GraphNode = Cast<UEdGraphNode>(SelectedObject);

		if (USSNarrativeGraphEditorGraphNode* NarrativeGraphNode = Cast<USSNarrativeGraphEditorGraphNode>(GraphNode))
		{
			if (NarrativeGraphNode->SourceNode)
			{
				NodeDetailsView->SetObject(NarrativeGraphNode->SourceNode);
				return;
			}
		}
	}

	// No valid selection → clear the node details panel
	NodeDetailsView->SetObject(nullptr);
}


void FSSNarrativeGraphEditor::RefreshGraphEditor()
{
}

void FSSNarrativeGraphEditor::OnNodeDetailsChanged(const FPropertyChangedEvent& InEvent)
{
	// Notify dirty state
	NotifyPackageDirty();
}

void FSSNarrativeGraphEditor::DeleteSelectedNodes()
{
	if (!GraphEditor.IsValid())
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error,
		       TEXT("[NarrativeGraph] Cannot delete nodes: GraphEditor is invalid."));
		return;
	}

	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();

	if (SelectedNodes.Num() == 0)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Warning, TEXT("[NarrativeGraph] No nodes selected for deletion."));
		return;
	}

	int32 DeletedCount = 0;

	for (UObject* SelectedObject : SelectedNodes)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(SelectedObject);
		if (!Node)
		{
			UE_LOG(LogSSNarrativeGraphEditor, Warning,
			       TEXT("[NarrativeGraph] Selected object '%s' is not a UEdGraphNode."),
			       *GetNameSafe(SelectedObject));
			continue;
		}

		Node->Modify(); // Allow undo/redo
		Node->DestroyNode(); // Remove from graph
		DeletedCount++;

		UE_LOG(LogSSNarrativeGraphEditor, Log,
		       TEXT("[NarrativeGraph] Deleted node: %s"), *Node->GetName());
	}

	UE_LOG(LogSSNarrativeGraphEditor, Log,
	       TEXT("[NarrativeGraph] Deleted %d node(s)."), DeletedCount);
}

bool FSSNarrativeGraphEditor::CanDeleteNodes() const
{
	return GraphEditor.IsValid() && GraphEditor->GetSelectedNodes().Num() > 0;
}

/////////////////////////////////////////////////////
// Compile 
FSlateIcon FSSNarrativeGraphEditor::GetCompileStatusIcon() const
{
	static const FName CompileStatusBackground("Blueprint.CompileStatus.Background");
	static const FName CompileStatusUnknown("Blueprint.CompileStatus.Overlay.Unknown");
	static const FName CompileStatusError("Blueprint.CompileStatus.Overlay.Error");
	static const FName CompileStatusGood("Blueprint.CompileStatus.Overlay.Good");
	static const FName CompileStatusWarning("Blueprint.CompileStatus.Overlay.Warning");

	switch (CompileStatus)
	{
	default:
	case BS_Unknown:
	case BS_Dirty:
		return FSlateIcon(FAppStyle::GetAppStyleSetName(), CompileStatusBackground, NAME_None, CompileStatusUnknown);
	case BS_Error:
		return FSlateIcon(FAppStyle::GetAppStyleSetName(), CompileStatusBackground, NAME_None, CompileStatusError);
	case BS_UpToDate:
		return FSlateIcon(FAppStyle::GetAppStyleSetName(), CompileStatusBackground, NAME_None, CompileStatusGood);
	case BS_UpToDateWithWarnings:
		return FSlateIcon(FAppStyle::GetAppStyleSetName(), CompileStatusBackground, NAME_None, CompileStatusWarning);
	}
}

FText FSSNarrativeGraphEditor::GetCompileStatusTooltip() const
{
	switch (CompileStatus)
	{
	case EBlueprintStatus::BS_UpToDate:
		return LOCTEXT("CompileStatus_Good", "Compiled successfully");
	case EBlueprintStatus::BS_Dirty:
		return LOCTEXT("CompileStatus_Dirty", "Graph has been modified");
	case EBlueprintStatus::BS_Error:
		return LOCTEXT("CompileStatus_Error", "Compile failed – check your nodes");
	default:
		return LOCTEXT("CompileStatus_Unknown", "Unknown compile status");
	}
}

void FSSNarrativeGraphEditor::NotifyPackageDirty()
{
	Asset->MarkPackageDirty();
	OnGraphAssetModified();
}

USSNarrativeGraphEditorGraphNode* FSSNarrativeGraphEditor::CreateVisualNode(USSNarrativeNode* RuntimeNode)
{
	if (!RuntimeNode)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Warning, TEXT("[NarrativeGraph] CreateVisualNode: RuntimeNode is null."));
		return nullptr;
	}

	if (!Asset)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error, TEXT("[NarrativeGraph] CreateVisualNode failed: Asset is null."));
		return nullptr;
	}

	if (!EdNarrativeGraph)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error,
		       TEXT("[NarrativeGraph] CreateVisualNode failed: EdNarrativeGraph is null."));
		return nullptr;
	}

	FGraphNodeCreator<USSNarrativeGraphEditorGraphNode> NodeCreator(*EdNarrativeGraph);
	USSNarrativeGraphEditorGraphNode* EdNode = NodeCreator.CreateNode();
	if (!EdNode)
	{
		UE_LOG(LogSSNarrativeGraphEditor, Error, TEXT("[NarrativeGraph] Failed to create visual graph node."));
		return nullptr;
	}

	EdNode->SetFlags(RF_Transactional);
	EdNode->SourceNode = RuntimeNode;
	EdNode->RuntimeGraph = Asset;
	EdNode->NodePosX = RuntimeNode->EditorPosition.X;
	EdNode->NodePosY = RuntimeNode->EditorPosition.Y;
	EdNode->NodeColor = RuntimeNode->GetEditorNodeColor();
	EdNode->AllocateDefaultPins();

	NodeCreator.Finalize();

	UE_LOG(LogSSNarrativeGraphEditor, Log,
	       TEXT("[NarrativeGraph] Visual node created for '%s' at (%d, %.d)."),
	       *RuntimeNode->GetName(), EdNode->NodePosX, EdNode->NodePosY);

	return EdNode;
}


#undef LOCTEXT_NAMESPACE
