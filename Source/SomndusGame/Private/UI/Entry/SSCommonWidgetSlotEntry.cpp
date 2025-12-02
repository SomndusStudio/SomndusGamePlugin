/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Entry/SSCommonWidgetSlotEntry.h"

#include "UI/SSBackgroundSlotWidget.h"
#include "UI/SSCommonFocusWidget.h"
#include "UI/SSCommonUIFunctionLibrary.h"
#include "UI/SSWidgetObjectEntry.h"


USSCommonWidgetSlotEntry::USSCommonWidgetSlotEntry()
{
	ObjectDataType = UObject::StaticClass();
}

UObject* USSCommonWidgetSlotEntry::GetCheckedItemObjectAs(TSubclassOf<UObject> Class) const
{
	return ItemObject;
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

void USSCommonWidgetSlotEntry::OnHover_Implementation(bool bInActive)
{
}

void USSCommonWidgetSlotEntry::PerformOnHover(bool bInActive)
{
	// Store
	if (bShouldStoreLastFocusedWidget)
	{
		USSCommonUIFunctionLibrary::StoreCacheFocusedWidget(this, bInActive ? this : nullptr);
	}
	OnHover(bInActive);
	OnHoverEvent.Broadcast(this, bInActive);
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
