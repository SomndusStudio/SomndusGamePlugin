// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintGraph/K2Node_SSGetGameModule.h"

#include "Core/SSCommonFunctionLibrary.h"
#include "Core/SSGameModule.h"

void UK2Node_SSGetGameModule::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	GetTemplateMenuActions<USSGameModule>(ActionRegistrar);
}

FText UK2Node_SSGetGameModule::GetMenuCategory() const
{
	if (CustomClass->IsChildOf<USSGameModule>())
	{
		return NSLOCTEXT("SomndusStudio", "GetGameModule_GameModuleMenuCategory", "Somndus Studio | Game Modules");
	}

	return NSLOCTEXT("SomndusStudio", "GetGameModule_InvalidGameModuleTypeMenuCategory", "Invalid Game Module Type");
}

FText UK2Node_SSGetGameModule::GetTooltipText() const
{
	if (CustomClass)
	{
		FText GameModuleTypeText =  NSLOCTEXT("SomndusStudio", "GetGameModule_InvalidGameModuleTypeTooltip", "Invalid GameModule Type");
		if (CustomClass->IsChildOf<USSGameModule>())
		{
			GameModuleTypeText =  NSLOCTEXT("SomndusStudio", "GetGameModule_GetGameModuleTooltip", "GameModule");
		}
		return FText::FormatNamed(NSLOCTEXT("SomndusStudio", "GetGameModule_TooltipFormat", "Get {ClassName} ({GameModuleType})\n\n{ClassTooltip}"),
			TEXT("ClassName"), CustomClass->GetDisplayNameText(),
			TEXT("GameModuleType"), GameModuleTypeText,
			TEXT("ClassTooltip"), CustomClass->GetToolTipText(/*bShortTooltip=*/ true));
	}

	return NSLOCTEXT("SomndusStudio", "GetGameModule_InvalidSubsystemTypeTooltip", "GameModule Type");
}

void UK2Node_SSGetGameModule::CreateInputCustomPin()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, USSGameModule::StaticClass(), TEXT("Class"));
}

void UK2Node_SSGetGameModule::CreateOutputCustomPin()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, (CustomClass ? (UClass*)CustomClass : USSGameModule::StaticClass()), UEdGraphSchema_K2::PN_ReturnValue);
}

FName UK2Node_SSGetGameModule::GetFunctionAccessName()
{
	if (CustomClass->IsChildOf<USSGameModule>())
	{
		return GET_FUNCTION_NAME_CHECKED(USSCommonFunctionLibrary, GetGameModule);
	}
	return FName();
}

TSubclassOf<UObject> UK2Node_SSGetGameModule::GetOutputClassObject()
{
	return USSGameModule::StaticClass();
}
