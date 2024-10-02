// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SSCommonWidgetSlotEntry.h"


USSCommonWidgetSlotEntry::USSCommonWidgetSlotEntry()
{
	ObjectDataType = UObject::StaticClass();
}

void USSCommonWidgetSlotEntry::OnItemObjectSet_Implementation(UObject* InItemObject)
{
	ISSWidgetObjectEntry::OnItemObjectSet_Implementation(InItemObject);
	
	ItemObject = InItemObject;

	OnItemObjectUpdate();
}

const UWidget* USSCommonWidgetSlotEntry::GetDefaultFocusWidget_Implementation()
{
	if (DefaultButtonSlot)
	{
		return DefaultButtonSlot;
	}
	return ISSCommonFocusWidget::GetDefaultFocusWidget_Implementation();
}

void USSCommonWidgetSlotEntry::OnItemObjectUpdate_Implementation()
{
}
