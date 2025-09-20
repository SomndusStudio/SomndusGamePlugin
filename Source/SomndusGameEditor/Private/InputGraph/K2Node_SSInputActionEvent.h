// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "K2Node_Event.h"
#include "InputAction.h"
#include "InputTriggers.h"
#include "EnhancedInputActionDelegateBinding.h"
#include "K2Node_SSInputActionEvent.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEEDITOR_API UK2Node_SSInputActionEvent : public UK2Node_Event
{
	GENERATED_UCLASS_BODY()
	

	UPROPERTY()
	TObjectPtr<const UInputAction> InputAction;

	UPROPERTY()
	ETriggerEvent TriggerEvent;

	// -- K2 Node Interface
	virtual void AllocateDefaultPins() override;
	virtual UClass* GetDynamicBindingClass() const override;
	virtual void RegisterDynamicBinding(UDynamicBlueprintBinding* BindingObject) const override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
};
