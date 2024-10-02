// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintGraph/K2Node_SSGetDAO.h"

#include "Core/SSCommonFunctionLibrary.h"
#include "DAO/SSDAOObjectBase.h"

void UK2Node_SSGetDAO::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	GetTemplateMenuActions<USSDAOObjectBase>(ActionRegistrar);
}

FText UK2Node_SSGetDAO::GetMenuCategory() const
{
	if (CustomClass->IsChildOf<USSDAOObjectBase>())
	{
		return NSLOCTEXT("SomndusStudio", "GetDAO_DAOMenuCategory", "Somndus Studio | DAO");
	}

	return NSLOCTEXT("SomndusStudio", "GetDAO_InvalidDAOTypeMenuCategory", "Invalid DAO Type");
}

FText UK2Node_SSGetDAO::GetTooltipText() const
{
	if (CustomClass)
	{
		FText GetDAOTypeText =  NSLOCTEXT("SomndusStudio", "GetGameModule_InvalidGameModuleTypeTooltip", "Invalid GameModule Type");
		if (CustomClass->IsChildOf<USSDAOObjectBase>())
		{
			GetDAOTypeText =  NSLOCTEXT("SomndusStudio", "GetDAO_GetDAOTooltip", "GameModule");
		}
		return FText::FormatNamed(NSLOCTEXT("SomndusStudio", "GetDAO_TooltipFormat", "Get {ClassName} ({DAOType})\n\n{ClassTooltip}"),
			TEXT("ClassName"), CustomClass->GetDisplayNameText(),
			TEXT("DAOType"), GetDAOTypeText,
			TEXT("ClassTooltip"), CustomClass->GetToolTipText(/*bShortTooltip=*/ true));
	}

	return NSLOCTEXT("SomndusStudio", "GetGameModule_InvalidSubsystemTypeTooltip", "GameModule Type");
}

void UK2Node_SSGetDAO::CreateInputCustomPin()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, USSDAOObjectBase::StaticClass(), TEXT("Class"));
}

void UK2Node_SSGetDAO::CreateOutputCustomPin()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, (CustomClass ? (UClass*)CustomClass : USSDAOObjectBase::StaticClass()), UEdGraphSchema_K2::PN_ReturnValue);
}

FName UK2Node_SSGetDAO::GetFunctionAccessName()
{
	if (CustomClass->IsChildOf<USSDAOObjectBase>())
	{
		return GET_FUNCTION_NAME_CHECKED(USSCommonFunctionLibrary, GetDAOByClass);
	}
	return FName();
}

TSubclassOf<UObject> UK2Node_SSGetDAO::GetOutputClassObject()
{
	return USSDAOObjectBase::StaticClass();
}
