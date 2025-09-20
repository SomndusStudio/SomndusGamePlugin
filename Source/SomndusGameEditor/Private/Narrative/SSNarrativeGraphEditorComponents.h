/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "ConnectionDrawingPolicy.h"
#include "EdGraphUtilities.h"
#include "SGraphNode.h"
#include "SGraphPanel.h"
#include "SGraphPin.h"
#include "EdGraph/EdGraph.h"
#include "Narrative/SSNarrativeNode.h"
#include "Narrative/SSNarrativeTypes.h"
#include "Widgets/SCompoundWidget.h"
#include "SSNarrativeGraphEditorComponents.generated.h"

#define LOCTEXT_NAMESPACE "RPGEditorModule"

class FSSNarrativeGraphEditor;
class USSNarrativeGraph;

class FRPGConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:

	FRPGConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj);

	virtual void DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params) override;
	virtual void DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint, const FConnectionParams& Params) override;
	virtual void DrawPreviewConnector(const FGeometry& PinGeometry, const FVector2D& StartPoint, const FVector2D& EndPoint, UEdGraphPin* Pin) override;
	virtual void DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params) override;
	virtual void DetermineLinkGeometry(FArrangedChildren& ArrangedNodes, TSharedRef<SWidget>& OutputPinWidget, UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FArrangedWidget*& StartWidgetGeometry, FArrangedWidget*& EndWidgetGeometry) override;
	virtual FVector2D ComputeSplineTangent(const FVector2D& Start, const FVector2D& End) const override;
	virtual void Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries, FArrangedChildren& ArrangedNodes) override;
};

UCLASS()
class USSNarrativeGraphEditorGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UPROPERTY()
	USSNarrativeGraph* RuntimeGraph;
	
	UPROPERTY()
	USSNarrativeNode* SourceNode;
	
	UPROPERTY()
	FLinearColor NodeColor = FLinearColor::White;
	
	FText GetDisplayName() const
	{
		return SourceNode->GetDisplayName();
	}
	
	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual void DestroyNode() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		if (!SourceNode)
		{
			return NSLOCTEXT("NarrativeGraph", "InvalidNode", "Invalid Node");
		}
		
		// Option : afficher le nom de la classe (ou ton propre label dans le node)
		return GetDisplayName();
	}
	//~ End UEdGraphNode Interface
	
};

UCLASS()
class USSNarrativeGraph_EdGraph : public UEdGraph
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	TWeakObjectPtr<USSNarrativeGraph> RuntimeGraph;

	/** Toolkit qui édite ce graph */
	TWeakPtr<FSSNarrativeGraphEditor> OwningToolkit;
	
	/** Getter safe */
	TSharedPtr<FSSNarrativeGraphEditor> GetOwningToolkit() const
	{
		return OwningToolkit.Pin();
	}
};

class SGraphPinNarrative : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SGraphPinNarrative) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
private:
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;

	virtual const FSlateBrush* GetPinIcon() const override;
	virtual FSlateColor GetPinColor() const override;

	const FSlateBrush* GetPinBorder() const;
	TOptional<EMouseCursor::Type> GetPinCursor() const;
};

class SSSNarrativeGraphNodeVisual : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SSSNarrativeGraphNodeVisual) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, USSNarrativeGraphEditorGraphNode* InNode);

	FText GetNodeTitle() const;
	
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

	bool bCanMove = true;
	bool CanMove() const;
	// SGraphNode override
	virtual void UpdateGraphNode() override;
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	
protected:
};

UCLASS()
class USSNarrativeGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:

	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	
	// Autorise la connexion entre deux pins
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override { return GT_StateMachine; }
};

class FSSNarrativeGraphNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* InNode) const override;
};

#undef LOCTEXT_NAMESPACE