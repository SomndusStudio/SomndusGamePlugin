/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "BlueprintGraph/K2Node_SSGetWidgetEntryItemObject.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "UI/SSCommonWidgetSlotEntry.h"
#include "UObject/UnrealTypePrivate.h"

void UK2Node_SSGetWidgetEntryItemObject::AllocateDefaultPins()
{
	bool bFoundClass = false;
	
	if (UBlueprint* OwningBlueprint = GetBlueprint())
	{
		// Récupérer le Blueprint
		if (auto* Generate = Cast<UBlueprintGeneratedClass>(OwningBlueprint->GeneratedClass))
		{
			if (auto* Property = Generate->FindPropertyByName(TEXT("ObjectDataType")))
			{
				// check(Property)
				FClassProperty* ClassProperty = CastField<FClassProperty>(Property);
				UE_LOG(LogTemp, Display, TEXT("%p"), ClassProperty);
				{
					// TODO
					///ClassProperty->GetClass()
					// check(ClassProperty)
				
					UClass* OutputClass = ClassProperty->MetaClass;
					auto Test = ClassProperty->GetPropertyValue_InContainer(Generate);
					//TObjectPtr<UClass> ClassValuePtr = ClassProperty->GetPropertyValue_InContainer(Generate);

					if (UObject* LoadedProperty = ClassProperty->LoadObjectPropertyValue(OwningBlueprint))
					{
						UClass* Value = Cast<UClass>(LoadedProperty);
					}
					
					FString PinName = FString::Printf(TEXT("%s (Casted)"), *Property->GetName());
					CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, OutputClass, *PinName);

					bFoundClass = true;
				}
			}
			
	
		}
		
	}
	
	if (!bFoundClass)
	{
		// Fallback au cas où ObjectDataType n'est pas défini
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, TEXT("ItemObject"));
	}

	Super::AllocateDefaultPins();
}

bool UK2Node_SSGetWidgetEntryItemObject::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
	return Super::IsCompatibleWithGraph(TargetGraph) &&
		(!Blueprint || FBlueprintEditorUtils::FindUserConstructionScript(Blueprint) != TargetGraph);
}

FText UK2Node_SSGetWidgetEntryItemObject::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	// Assuming the WorldContextObject is USSCommonWidgetSlotEntry
	/*
	if (const USSCommonWidgetSlotEntry* WidgetSlotEntry = Cast<USSCommonWidgetSlotEntry>(this))
	{
		if (WidgetSlotEntry->ItemObject && WidgetSlotEntry->ObjectDataType)
		{
			if (TitleType == ENodeTitleType::FullTitle)
			{
				return CustomClass->GetDisplayNameText();
			}

			const FString& ClassName = CustomClass->GetName();
			return FText::FormatNamed(NSLOCTEXT("K2Node", "GetGameContextSubsystem_NodeTitleFormat", "Get {ClassName} Data"), TEXT("ClassName"), FText::FromString(ClassName));
		}
	}
	*/
	
	return FText::FromString(TEXT("Get Casted Item Object"));
}

FText UK2Node_SSGetWidgetEntryItemObject::GetTooltipText() const
{
	return FText::FromString(TEXT("Casts the ItemObject from the USSCommonWidgetSlotEntry to the specified ObjectDataType."));
}

void UK2Node_SSGetWidgetEntryItemObject::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	// Find function to call
	UFunction* Function = FindUField<UFunction>(UK2Node_SSGetWidgetEntryItemObject::StaticClass(), GET_FUNCTION_NAME_CHECKED(UK2Node_SSGetWidgetEntryItemObject, GetCastedItemObject));
	if (!Function)
	{
		CompilerContext.MessageLog.Error(TEXT("Unable to find GetCastedItemObject function"));
		return;
	}

	// Create a call function node to invoke GetCastedItemObject
	UK2Node_CallFunction* CallFunctionNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallFunctionNode->SetFromFunction(Function);
	CallFunctionNode->AllocateDefaultPins();

	// Connect the WorldContextObject to the current context (this node only works in USSCommonWidgetSlotEntry context)
	CompilerContext.MovePinLinksToIntermediate(*FindPin(UEdGraphSchema_K2::PN_Self), *CallFunctionNode->FindPin(UEdGraphSchema_K2::PN_Self));
    
	// Connect the return value pin from the intermediate node to the output of this node
	CompilerContext.MovePinLinksToIntermediate(*FindPin(TEXT("CastedItemObject")), *CallFunctionNode->GetReturnValuePin());

	BreakAllNodeLinks();
}

void UK2Node_SSGetWidgetEntryItemObject::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// Vérifier si ce nœud est déjà enregistré
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		// Créer un nouveau spawner de nœud
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		if (NodeSpawner != nullptr)
		{
			// Enregistrer le spawner dans la base de données d'actions
			ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
		}
	}
}

FText UK2Node_SSGetWidgetEntryItemObject::GetMenuCategory() const
{
	return NSLOCTEXT("K2Node", "Utility_SomndusStudio", "Somndus Studio - Utility");
}

UObject* UK2Node_SSGetWidgetEntryItemObject::GetCastedItemObject(UObject* WorldContextObject)
{
	// Assuming the WorldContextObject is USSCommonWidgetSlotEntry
	if (USSCommonWidgetSlotEntry* WidgetSlotEntry = Cast<USSCommonWidgetSlotEntry>(WorldContextObject))
	{
		// Get ItemObject and cast it to the appropriate class type
		if (WidgetSlotEntry->GetItemObject() && WidgetSlotEntry->ObjectDataType)
		{
			return WidgetSlotEntry->GetItemObject()->IsA(WidgetSlotEntry->ObjectDataType) ? WidgetSlotEntry->GetItemObject() : nullptr;
		}
	}
	return nullptr;
}
