/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "BlueprintGraph/K2Node_SSCustomClassAccessBase.h"

#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "KismetCompilerMisc.h"
#include "Kismet2/BlueprintEditorUtils.h"

void UK2Node_SSCustomClassAccessBase::Serialize(FArchive& Ar)
{
	Super::Serialize( Ar );

	if (CustomClass == nullptr)
	{
		if (UEdGraphPin* ClassPin = FindPin(TEXT("Class"), EGPD_Input))
		{
			ClassPin->SetSavePinIfOrphaned(false);
		}
	}
}

void UK2Node_SSCustomClassAccessBase::Initialize(UClass* NodeClass)
{
	CustomClass = NodeClass;
}

void UK2Node_SSCustomClassAccessBase::AllocateDefaultPins()
{
	// If required add the world context pin
	if (GetBlueprint()->ParentClass->HasMetaDataHierarchical(FBlueprintMetadata::MD_ShowWorldContextPin))
	{
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), TEXT("WorldContext"));
	}

	// Add blueprint pin
	if (!CustomClass)
	{
		CreateInputCustomPin();
	}

	// Result pin
	CreateOutputCustomPin();

	Super::AllocateDefaultPins();
}

bool UK2Node_SSCustomClassAccessBase::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
	return Super::IsCompatibleWithGraph(TargetGraph) &&
		(!Blueprint || FBlueprintEditorUtils::FindUserConstructionScript(Blueprint) != TargetGraph);
}

FSlateIcon UK2Node_SSCustomClassAccessBase::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

FLinearColor UK2Node_SSCustomClassAccessBase::GetNodeTitleColor() const
{
	return FLinearColor(1.f, 0.078f, 0.576f, 1.f);
}

FText UK2Node_SSCustomClassAccessBase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (CustomClass)
	{
		if (TitleType == ENodeTitleType::FullTitle)
		{
			return CustomClass->GetDisplayNameText();
		}

		const FString& ClassName = CustomClass->GetName();
		return FText::FormatNamed(NSLOCTEXT("SomndusStudio", "GetObject_NodeTitleFormat", "Get {ClassName}"), TEXT("ClassName"), FText::FromString(ClassName));
	}

	return GetTooltipText();
}

void UK2Node_SSCustomClassAccessBase::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	static const FName WorldContextObject_ParamName(TEXT("ContextObject"));
	static const FName Class_ParamName(TEXT("Class"));

	UK2Node_SSCustomClassAccessBase* GetSubsystemNode = this;
	UEdGraphPin* SpawnWorldContextPin = GetSubsystemNode->GetWorldContextPin();
	UEdGraphPin* SpawnClassPin = GetSubsystemNode->GetClassPin();
	UEdGraphPin* SpawnNodeResult = GetSubsystemNode->GetResultPin();

	UClass* SpawnClass = (SpawnClassPin != nullptr) ? Cast<UClass>(SpawnClassPin->DefaultObject) : nullptr;
	if (SpawnClassPin && (SpawnClassPin->LinkedTo.Num() == 0) && !SpawnClass)
	{
		CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node @@ must have a class specified.").ToString(), GetSubsystemNode);
		GetSubsystemNode->BreakAllNodeLinks();
		return;
	}

	// Choose appropriate underlying Getter
	FName Get_FunctionName = GetFunctionAccessName();
	if (Get_FunctionName.IsNone())
	{
		CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node @@ must have a class specified.").ToString(), GetSubsystemNode);
		GetSubsystemNode->BreakAllNodeLinks();
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// create 'USubsystemBlueprintLibrary::Get[something]Subsystem' call node
	UK2Node_CallFunction* CallGetNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(GetSubsystemNode, SourceGraph);
	CallGetNode->FunctionReference.SetExternalMember(Get_FunctionName, GetOutputClassObject());
	CallGetNode->AllocateDefaultPins();

	UEdGraphPin* CallCreateWorldContextPin = CallGetNode->FindPinChecked(WorldContextObject_ParamName);
	UEdGraphPin* CallCreateClassTypePin = CallGetNode->FindPinChecked(Class_ParamName);
	UEdGraphPin* CallCreateResult = CallGetNode->GetReturnValuePin();

	if (SpawnClassPin && SpawnClassPin->LinkedTo.Num() > 0)
	{
		// Copy the 'class' connection from the spawn node to 'USubsystemBlueprintLibrary::Get[something]Subsystem'
		CompilerContext.MovePinLinksToIntermediate(*SpawnClassPin, *CallCreateClassTypePin);
	}
	else
	{
		// Copy class literal onto 'USubsystemBlueprintLibrary::Get[something]Subsystem' call
		CallCreateClassTypePin->DefaultObject = *CustomClass;
	}

	// Copy the world context connection from the spawn node to 'USubsystemBlueprintLibrary::Get[something]Subsystem' if necessary
	if (SpawnWorldContextPin)
	{
		CompilerContext.MovePinLinksToIntermediate(*SpawnWorldContextPin, *CallCreateWorldContextPin);
	}

	// Move result connection from spawn node to 'USubsystemBlueprintLibrary::Get[something]Subsystem'
	CallCreateResult->PinType = SpawnNodeResult->PinType;
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeResult, *CallCreateResult);

	//////////////////////////////////////////////////////////////////////////

	// Break any links to the expanded node
	GetSubsystemNode->BreakAllNodeLinks();
}


UEdGraphPin* UK2Node_SSCustomClassAccessBase::GetClassPin(const TArray<UEdGraphPin*>* InPinsToSearch) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;

	UEdGraphPin* Pin = NULL;
	for (auto PinIt = PinsToSearch->CreateConstIterator(); PinIt; ++PinIt)
	{
		UEdGraphPin* TestPin = *PinIt;
		if (TestPin && TestPin->PinName == TEXT("Class"))
		{
			Pin = TestPin;
			break;
		}
	}

	return Pin;
}


void UK2Node_SSCustomClassAccessBase::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	if (!CustomClass)
	{
		if (auto ClassNode = GetClassPin(&OldPins))
		{
			CustomClass = Cast<UClass>(ClassNode->DefaultObject);
		}
	}

	AllocateDefaultPins();

	if (CustomClass)
	{
		UEdGraphPin* ResultPin = GetResultPin();
		ResultPin->PinType.PinSubCategoryObject = *CustomClass;
	}
}

class FNodeHandlingFunctor* UK2Node_SSCustomClassAccessBase::CreateNodeHandler(
	FKismetCompilerContext& CompilerContext) const
{
	return new FNodeHandlingFunctor(CompilerContext);
}

void UK2Node_SSCustomClassAccessBase::GetNodeAttributes(
	TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const
{
	const FString ClassToSpawnStr = CustomClass ? CustomClass->GetName() : TEXT("InvalidClass");
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Type"), TEXT("GetObject")));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Class"), GetClass()->GetName()));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Name"), GetName()));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("ObjectClass"), ClassToSpawnStr));
}


UEdGraphPin* UK2Node_SSCustomClassAccessBase::GetWorldContextPin() const
{
	UEdGraphPin* Pin = FindPin(TEXT("WorldContext"));
	check(Pin == NULL || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_SSCustomClassAccessBase::GetResultPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

void UK2Node_SSCustomClassAccessBase::CreateInputCustomPin()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, UObject::StaticClass(), TEXT("Class"));
}

void UK2Node_SSCustomClassAccessBase::CreateOutputCustomPin()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, (CustomClass ? (UClass*)CustomClass : UObject::StaticClass()), UEdGraphSchema_K2::PN_ReturnValue);
}

FName UK2Node_SSCustomClassAccessBase::GetFunctionAccessName()
{
	return FName();
}

TSubclassOf<UObject> UK2Node_SSCustomClassAccessBase::GetOutputClassObject()
{
	return UObject::StaticClass();
}
