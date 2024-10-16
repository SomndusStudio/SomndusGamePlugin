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

void USSCommonWidgetSlotEntry::PerformOnHover(bool bActive)
{
	OnHoverEvent.Broadcast(this, bActive);
}

void USSCommonWidgetSlotEntry::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	PerformOnHover(true);
}

void USSCommonWidgetSlotEntry::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	PerformOnHover(false);
}

void USSCommonWidgetSlotEntry::OnItemObjectUpdate_Implementation()
{
}
