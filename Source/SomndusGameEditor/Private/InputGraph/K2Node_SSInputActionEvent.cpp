// ©2025 Schartier Isaac. All rights reserved.


#include "K2Node_SSInputActionEvent.h"

#include "EdGraphSchema_K2.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Input/Context/SSInputHandlerContext.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EnhancedInputActionDelegateBinding.h"

#define LOCTEXT_NAMESPACE "K2Node"
UK2Node_SSInputActionEvent::UK2Node_SSInputActionEvent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bInternalEvent = true;
    TriggerEvent = ETriggerEvent::Triggered;
}

void UK2Node_SSInputActionEvent::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
}

UClass* UK2Node_SSInputActionEvent::GetDynamicBindingClass() const
{
    return UEnhancedInputActionDelegateBinding::StaticClass();
}

void UK2Node_SSInputActionEvent::RegisterDynamicBinding(UDynamicBlueprintBinding* BindingObject) const
{
    UEnhancedInputActionDelegateBinding* InputBinding = CastChecked<UEnhancedInputActionDelegateBinding>(BindingObject);

    FBlueprintEnhancedInputActionBinding Binding;
    Binding.InputAction = InputAction;
    Binding.TriggerEvent = TriggerEvent;
    Binding.FunctionNameToBind = CustomFunctionName;

    InputBinding->InputActionDelegateBindings.Add(Binding);
}

bool UK2Node_SSInputActionEvent::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
    if (TargetGraph)
    {
        UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
        if (Blueprint && Blueprint->ParentClass->IsChildOf(USSInputHandlerContext::StaticClass()))
        {
            return true;
        }
    }

    return false;
}

void UK2Node_SSInputActionEvent::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
    for (TObjectIterator<UInputAction> It; It; ++It)
    {
        UInputAction* InputActionAsset = *It;

        if (!IsValid(InputActionAsset)) continue;

        auto NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());

        NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateLambda(
            [InputActionAsset](UEdGraphNode* NewNode, bool bIsTemplateNode)
            {
                UK2Node_SSInputActionEvent* EventNode = CastChecked<UK2Node_SSInputActionEvent>(NewNode);
                EventNode->InputAction = InputActionAsset;
                EventNode->TriggerEvent = ETriggerEvent::Triggered;
            });

        ActionRegistrar.AddBlueprintAction(GetClass(), NodeSpawner);
    }
}

FText UK2Node_SSInputActionEvent::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (InputAction)
    {
        return FText::Format(LOCTEXT("InputActionEventNodeTitle", "Event {0} ({1})"),
            FText::FromName(InputAction->GetFName()),
            FText::FromString(UEnum::GetValueAsString(TriggerEvent))
        );
    }

    return LOCTEXT("InputActionEventNodeDefault", "Input Action Event");
}

FText UK2Node_SSInputActionEvent::GetTooltipText() const
{
    return LOCTEXT("SSInputActionEventTooltip", "Handles an Enhanced Input Action for this Input Context.");
}

#undef LOCTEXT_NAMESPACE
