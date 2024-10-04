/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_SSGetWidgetEntryItemObject.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEEDITOR_API UK2Node_SSGetWidgetEntryItemObject : public UK2Node
{
	GENERATED_BODY()
public:
	// Override to define node functionality
	virtual void AllocateDefaultPins() override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	
	// Override to provide node behavior in the editor
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual bool IsNodePure() const { return true; }
	
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	
	// Function to retrieve the casted object
	static UObject* GetCastedItemObject(UObject* WorldContextObject);
	
protected:
	UPROPERTY()
	TSubclassOf<UObject> CustomClass;
};
