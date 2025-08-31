/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node.h"
#include "K2Node_SSCustomClassAccessBase.generated.h"

/**
 * Base class for K2 nodes that provide custom class access functionality.
 * This abstract class is intended to be extended for specific use cases.
 */
UCLASS(Abstract)
class SOMNDUSGAMEEDITOR_API UK2Node_SSCustomClassAccessBase : public UK2Node
{
	GENERATED_BODY()

public:
	virtual void Serialize(FArchive& Ar) override;

	void Initialize(UClass* NodeClass);
	
	// Override to define node functionality
	virtual void AllocateDefaultPins() override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	
	// Override to provide node behavior in the editor
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual bool IsNodePure() const { return true; }
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;

	virtual void GetNodeAttributes(TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const override;


	/** Get the blueprint input pin */
	UEdGraphPin* GetClassPin(const TArray< UEdGraphPin* >* InPinsToSearch = nullptr) const;
	/** Get the world context input pin, can return NULL */
	UEdGraphPin* GetWorldContextPin() const;

	/** Get the result output pin */
	UEdGraphPin* GetResultPin() const;

	virtual bool ShouldDrawCompact() const override { return true; }
	
	virtual void CreateInputCustomPin();
	virtual void CreateOutputCustomPin();
	virtual FName GetFunctionAccessName();
	virtual TSubclassOf<class UObject> GetOutputClassObject();
	
	template<class T>
	void GetTemplateMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const;

protected:
	
	UPROPERTY()
	TSubclassOf<UObject> CustomClass;
	
};

template <class T>
void UK2Node_SSCustomClassAccessBase::GetTemplateMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	static TArray<UClass*> Subclasses;
	Subclasses.Reset();
	GetDerivedClasses(T::StaticClass(), Subclasses);
	auto CustomizeCallback = [](UEdGraphNode* Node, bool bIsTemplateNode, UClass* Subclass)
	{
		auto TypedNode = CastChecked<UK2Node_SSCustomClassAccessBase>(Node);
		TypedNode->Initialize(Subclass);
	};

	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		for (UClass* Iter : Subclasses)
		{
			if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Iter, true))
			{
				continue;
			}

			UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(ActionKey);
			check(Spawner);

			Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeCallback, Iter);
			ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
		}
	}
}
