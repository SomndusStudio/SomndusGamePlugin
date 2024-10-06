/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSCommonWidgetSlotEntry.h"


USSCommonWidgetSlotEntry::USSCommonWidgetSlotEntry()
{
	ObjectDataType = UObject::StaticClass();
}

void USSCommonWidgetSlotEntry::OnItemObjectSet_Implementation(UObject* InItemObject)
{
	ISSWidgetObjectEntry::OnItemObjectSet_Implementation(InItemObject);

	PreOnItemObjectUpdate();
	
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

void USSCommonWidgetSlotEntry::PreOnItemObjectUpdate_Implementation()
{
}

void USSCommonWidgetSlotEntry::PerformOnClick()
{
	OnClickEvent.Broadcast(this);
}

void USSCommonWidgetSlotEntry::OnItemObjectUpdate_Implementation()
{
}
