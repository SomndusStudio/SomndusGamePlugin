/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#include "Narrative/SSNarrativeGraphEditorComponents.h"

#include "ConnectionDrawingPolicy.h"
#include "SGraphPanel.h"
#include "SGraphPin.h"
#include "SPinTypeSelector.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Narrative/SSNarrativeGraphEditor.h"
#include "Narrative/SSNarrativeNode_Start.h"
#include "Styling/SlateStyleRegistry.h"

FRPGConnectionDrawingPolicy::FRPGConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor,
                                                         const FSlateRect& InClippingRect,
                                                         FSlateWindowElementList& InDrawElements,
                                                         UEdGraph* InGraphObj): FConnectionDrawingPolicy(
	InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements)
{
}

void FRPGConnectionDrawingPolicy::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom,
                                                      const FConnectionParams& Params)
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom) - FVector2D(0.0, 4.f);

	DrawSplineWithArrow(StartCenter, EndCenter, Params);
}

void FRPGConnectionDrawingPolicy::DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint,
                                                      const FConnectionParams& Params)
{
	// Draw a line/spline
	DrawConnection(WireLayerID, StartPoint, EndPoint, Params);

	// Draw the arrow
	if (ArrowImage != nullptr)
	{
		// Draw arrow manually facing downward
		const FVector2D ArrowPos = EndPoint - ArrowRadius;

		// Force vertical arrow (down)
		const float AngleInRadians = PI * 0.5f;

		// Draw arrow
		FSlateDrawElement::MakeRotatedBox(
			DrawElementsList,
			ArrowLayerID,
			FPaintGeometry(
				ArrowPos,
				ArrowImage->ImageSize * ZoomFactor,
				ZoomFactor
			),
			ArrowImage,
			ESlateDrawEffect::None,
			AngleInRadians,
			TOptional<FVector2D>(),
			FSlateDrawElement::RelativeToElement,
			Params.WireColor
		);
	}
}

void FRPGConnectionDrawingPolicy::Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries,
                                       FArrangedChildren& ArrangedNodes)
{
	FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes);
}

void FRPGConnectionDrawingPolicy::DrawPreviewConnector(const FGeometry& PinGeometry, const FVector2D& StartPoint,
                                                       const FVector2D& EndPoint, UEdGraphPin* Pin)
{
	FConnectionParams Params;
	DetermineWiringStyle(Pin, nullptr, /*inout*/ Params);

	const FVector2D SeedPoint = EndPoint;
	const FVector2D AdjustedStartPoint = FGeometryHelper::FindClosestPointOnGeom(PinGeometry, SeedPoint);

	DrawSplineWithArrow(AdjustedStartPoint, EndPoint, Params);
}

void FRPGConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin,
                                                       FConnectionParams& Params)
{
	FConnectionDrawingPolicy::DetermineWiringStyle(OutputPin, InputPin, Params);

	Params.AssociatedPin1 = OutputPin;
	Params.AssociatedPin2 = InputPin;
	Params.WireThickness = 1.5f;

	Params.WireColor = FLinearColor::White;
}

void FRPGConnectionDrawingPolicy::DetermineLinkGeometry(FArrangedChildren& ArrangedNodes,
                                                        TSharedRef<SWidget>& OutputPinWidget, UEdGraphPin* OutputPin,
                                                        UEdGraphPin* InputPin,
                                                        FArrangedWidget*& StartWidgetGeometry,
                                                        FArrangedWidget*& EndWidgetGeometry)
{
	FConnectionDrawingPolicy::DetermineLinkGeometry(ArrangedNodes, OutputPinWidget, OutputPin, InputPin,
	                                                StartWidgetGeometry, EndWidgetGeometry);
}

FVector2D FRPGConnectionDrawingPolicy::ComputeSplineTangent(const FVector2D& Start, const FVector2D& End) const
{
	// Use vertical flow
	const float Tension = 0.5f;
	return FVector2D(0.f, (End.Y - Start.Y) * Tension);
}

void USSNarrativeGraphEditorGraphNode::AllocateDefaultPins()
{
	const FName PinCategory = TEXT("Narrative");

	FEdGraphPinType NarrativePinType;
	NarrativePinType.PinCategory = PinCategory;
	NarrativePinType.PinSubCategory = NAME_None;
	NarrativePinType.PinSubCategoryObject = nullptr;
	NarrativePinType.ContainerType = EPinContainerType::None;
	NarrativePinType.bIsReference = false;
	NarrativePinType.bIsConst = false;

	if (!SourceNode) return;

	// Check if it's the start node (root)
	if (SourceNode->IsA(USSNarrativeNode_Start::StaticClass()))
	{
		// Start node: only output
		UEdGraphPin* Out = CreatePin(EGPD_Output, NarrativePinType, TEXT("Out"));
	}
	else
	{
		// Default: input and output
		CreatePin(EGPD_Input, NarrativePinType, TEXT("In"));
		CreatePin(EGPD_Output, NarrativePinType, TEXT("Out"));
	}
}

void USSNarrativeGraphEditorGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != NULL)
	{
		if (GetSchema()->TryCreateConnection(FromPin, Pins[0]))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

void USSNarrativeGraphEditorGraphNode::DestroyNode()
{
	Super::DestroyNode();

	// Remove the logical node from the asset
	if (USSNarrativeGraph_EdGraph* Graph = Cast<USSNarrativeGraph_EdGraph>(GetGraph()))
	{
		if (RuntimeGraph)
		{
			RuntimeGraph->RemoveNode(SourceNode);
		}
	}
}

void SGraphPinNarrative::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor(EMouseCursor::Hand);

	bShowLabel = false;

	GraphPinObj = InPin;
	check(GraphPinObj != NULL);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);
	
	// Create the pin icon widget
	TSharedRef<SWidget> PinWidgetRef = SPinTypeSelector::ConstructPinTypeImage(
		MakeAttributeSP(this, &SGraphPinNarrative::GetPinIcon ),
		MakeAttributeSP(this, &SGraphPinNarrative::GetPinColor),
		MakeAttributeSP(this, &SGraphPinNarrative::GetSecondaryPinIcon),
		MakeAttributeSP(this, &SGraphPinNarrative::GetSecondaryPinColor));

	SetPinImageWidget(PinWidgetRef);
	
	// Set up a hover for pins that is tinted the color of the pin.
	SBorder::Construct(SBorder::FArguments()
	                   .BorderImage(this, &SGraphPinNarrative::GetPinBorder)
	                   .BorderBackgroundColor(this, &SGraphPinNarrative::GetPinColor)
	                   .OnMouseButtonDown(this, &SGraphPinNarrative::OnPinMouseDown)
	                   .Cursor(this, &SGraphPinNarrative::GetPinCursor)
	                   .Content()
		[

			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				GetPinImageWidget().ToSharedRef()
			]
		]
	);
}

TOptional<EMouseCursor::Type> SGraphPinNarrative::GetPinCursor() const
{
	check(PinImage.IsValid());

	if (PinImage->IsHovered())
	{
		if (bIsMovingLinks)
		{
			return EMouseCursor::GrabHandClosed;
		}
		return EMouseCursor::Hand;
	}
	else
	{
		return EMouseCursor::Default;
	}
}

TSharedRef<SWidget> SGraphPinNarrative::GetDefaultValueWidget()
{
	return SGraphPin::GetDefaultValueWidget();
}

const FSlateBrush* SGraphPinNarrative::GetPinIcon() const
{
	return FSlateStyleRegistry::FindSlateStyle("SSStyle")->GetBrush("NarrativeGraph.PinIcon.Tiny");
}

FSlateColor SGraphPinNarrative::GetPinColor() const
{
	return FLinearColor::White;
}

const FSlateBrush* SGraphPinNarrative::GetPinBorder() const
{
	return (IsHovered())
		       ? FSlateStyleRegistry::FindSlateStyle("SSStyle")->GetBrush("NarrativeGraph.PinHover.Tiny")
		       : GetPinIcon();
}


void SSSNarrativeGraphNodeVisual::Construct(const FArguments& InArgs, USSNarrativeGraphEditorGraphNode* InNode)
{
	GraphNode = InNode;

	ensure(GraphNode != nullptr);
	ensure(GraphNode->IsValidLowLevelFast());

	// Determine if can move cause root node
	auto* NarrativeGraphNode = Cast<USSNarrativeGraphEditorGraphNode>(GraphNode);
	check(NarrativeGraphNode->SourceNode);
	
	bCanMove = !NarrativeGraphNode->SourceNode->IsA(USSNarrativeNode_Start::StaticClass());
	
	UpdateGraphNode();
}

FText SSSNarrativeGraphNodeVisual::GetNodeTitle() const
{
	if (!GraphNode)
	{
		return FText::FromString(TEXT("Invalid GraphNode"));
	}
	 
	auto* NarrativeGraphNode = Cast<USSNarrativeGraphEditorGraphNode>(GraphNode);
	if (!NarrativeGraphNode)
	{
		return FText::FromString(TEXT("Invalid NarrativeGraphNode"));
	}
	 
	return NarrativeGraphNode->GetDisplayName();
}

TSharedPtr<SGraphPin> SSSNarrativeGraphNodeVisual::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SGraphPinNarrative, Pin);
}

bool SSSNarrativeGraphNodeVisual::CanMove() const
{
	return bCanMove;
}

void SSSNarrativeGraphNodeVisual::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	LeftNodeBox.Reset();
	RightNodeBox.Reset();

	auto* NarrativeGraphNode = Cast<USSNarrativeGraphEditorGraphNode>(GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

	this->GetOrAddSlot(ENodeZone::Center)
	[
		SNew(SBox)
		.MinDesiredWidth(250)
		[
			SNew(SOverlay)

			// LAYER 0: Le corps du node (background + titre + contenu)
			+ SOverlay::Slot()
			[
				SNew(SBorder)
				.BorderBackgroundColor(NarrativeGraphNode->NodeColor)
				.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
				.Padding(0)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(8, 20)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(this, &SSSNarrativeGraphNodeVisual::GetNodeTitle)
						.TextStyle(FAppStyle::Get(), "Graph.Node.NodeTitle")
					]
				]
			]

			// LAYER 1: Pins en haut
			+ SOverlay::Slot()
			.Padding(0, -10, 0, 0) // <- les pins débordent en haut
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SAssignNew(LeftNodeBox, SVerticalBox)
			]

			// LAYER 2: Pins en bas
			+ SOverlay::Slot()
			.Padding(0, 0, 0, -10) // <- les pins débordent en bas
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		]
	];

	CreatePinWidgets();
}

void SSSNarrativeGraphNodeVisual::MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	if (!CanMove()) return;
	SGraphNode::MoveTo(NewPosition, NodeFilter, bMarkDirty);
}

void USSNarrativeGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	USSNarrativeGraph_EdGraph* EditorGraph = CastChecked<USSNarrativeGraph_EdGraph>(&Graph);

	// Récupérer le runtime asset lié à ce graphe
	USSNarrativeGraph* RuntimeGraph = EditorGraph->RuntimeGraph.Get();
	if (!RuntimeGraph)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing runtime USSNarrativeGraph when initializing editor graph"));
		return;
	}

	auto* RootNode = RuntimeGraph->GetRootNode();

	//  Crée le node runtime Start
	if (!RootNode)
	{
		USSNarrativeNode_Start* RuntimeStartNode = NewObject<USSNarrativeNode_Start>(
			RuntimeGraph, USSNarrativeNode_Start::StaticClass());
		RuntimeStartNode->EditorPosition = FVector2D(0.f, 0.f);

		RootNode = RuntimeGraph->AllNodes.Add_GetRef(RuntimeStartNode);
	}

	//  Crée le node visuel lié
	FGraphNodeCreator<USSNarrativeGraphEditorGraphNode> NodeCreator(Graph);
	USSNarrativeGraphEditorGraphNode* EntryNode = NodeCreator.CreateNode();

	EntryNode->SetFlags(RF_Transactional);
	EntryNode->NodeColor = RootNode->GetEditorNodeColor();
	EntryNode->NodePosX = RootNode->EditorPosition.X;
	EntryNode->NodePosY = RootNode->EditorPosition.Y;
	EntryNode->SourceNode = RootNode;

	EntryNode->AllocateDefaultPins();

	NodeCreator.Finalize();

	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);
}

class FSSNarrativeSchemaAction_AddNode : public FEdGraphSchemaAction
{
public:
	TSubclassOf<USSNarrativeNode> NodeClass;

	FSSNarrativeSchemaAction_AddNode(TSubclassOf<USSNarrativeNode> InNodeClass)
		: FEdGraphSchemaAction(
			  FText::FromString("Add Narrative Node"),
			  FText::FromString(InNodeClass->GetName().Replace(TEXT("USSNarrativeNode_"), TEXT(""))),
			  FText::GetEmpty(),
			  0),
		  NodeClass(InNodeClass)
	{
	}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
	                                    bool bSelectNewNode  = true) override
	{
		if (auto* NarrativeGraph = Cast<USSNarrativeGraph_EdGraph>(ParentGraph))
		{
			if (auto Toolkit = NarrativeGraph->GetOwningToolkit())
			{
				Toolkit->HandleAddNodeFromContextMenu(NodeClass, FromPin, Location);
			}
		}
		return nullptr;
	}
};

const FPinConnectionResponse USSNarrativeGraphSchema::CanCreateConnection(const UEdGraphPin* A,
                                                                           const UEdGraphPin* B) const
{
	UE_LOG(LogTemp, Warning, TEXT("Checking connection: %s -> %s"), *A->PinName.ToString(), *B->PinName.ToString());

	if (A == B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pin to itself"));
	}

	// Ne pas connecter deux entrées ou deux sorties
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins must be input/output"));
	}

	// Autorise la connexion
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool USSNarrativeGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	if (CanCreateConnection(A, B).Response == CONNECT_RESPONSE_MAKE)
	{
		A->MakeLinkTo(B);
		return true;
	}
	return false;
}

void USSNarrativeGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	if (!ContextMenuBuilder.CurrentGraph) return;
	TArray<UClass*> NodeClasses;
	GetDerivedClasses(USSNarrativeNode::StaticClass(), NodeClasses, true);

	for (UClass* NodeClass : NodeClasses)
	{
		if (!NodeClass
			|| NodeClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists)
			|| NodeClass->GetName().StartsWith("SKEL_")
			|| NodeClass->GetName().StartsWith("REINST_"))
		{
			continue;
		}

		TSharedPtr<FSSNarrativeSchemaAction_AddNode> Action =
			MakeShared<FSSNarrativeSchemaAction_AddNode>(NodeClass);

		ContextMenuBuilder.AddAction(Action);
	}
}

TSharedPtr<SGraphNode> FSSNarrativeGraphNodeFactory::CreateNode(UEdGraphNode* InNode) const
{
	if (auto* VCNode = Cast<USSNarrativeGraphEditorGraphNode>(InNode))
	{
		TSharedRef<SSSNarrativeGraphNodeVisual> Widget = SNew(SSSNarrativeGraphNodeVisual, VCNode);
		
		UE_LOG(LogTemp, Log, TEXT("Created visual node : %s"), *VCNode->GetDisplayName().ToString());
		return Widget;
	}
	return nullptr;
}

class FConnectionDrawingPolicy* USSNarrativeGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID,
                                                                                        int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect,
                                                                                        class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	// Provide a basic drawing policy to allow wire preview rendering
	return new FRPGConnectionDrawingPolicy(
		InBackLayerID,
		InFrontLayerID,
		InZoomFactor,
		InClippingRect,
		InDrawElements, InGraphObj);
}
