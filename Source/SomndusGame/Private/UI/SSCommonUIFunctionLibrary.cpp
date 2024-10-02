// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SSCommonUIFunctionLibrary.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "UI/SSWidgetObjectEntry.h"

void USSCommonUIFunctionLibrary::SetItemObject(UUserWidget* EntryWidget, UObject* InItemObject)
{
	if (ISSWidgetObjectEntry* NativeImplementation = Cast<ISSWidgetObjectEntry>(EntryWidget))
	{
		NativeImplementation->NativeOnItemObjectSet(InItemObject);
	}
	else if (EntryWidget->Implements<USSWidgetObjectEntry>())
	{
		ISSWidgetObjectEntry::Execute_OnItemObjectSet(EntryWidget, InItemObject);
	}
}
