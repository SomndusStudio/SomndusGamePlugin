/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Cursor/SSWidgetSlotEntry_WithCursor.h"

#include "UI/SSCommonUIFunctionLibrary.h"

FSSCursorRequestInfo USSWidgetSlotEntry_WithCursor::GetCursorRequestInfo_Implementation()
{
	if (HandCursorCapture)
	{
		return HandCursorCapture->GetCursorRequestInfo();
	}
	return ISSCursorHandler::GetCursorRequestInfo_Implementation();
}

void USSWidgetSlotEntry_WithCursor::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	USSCommonUIFunctionLibrary::ShowCursor(this, this);
}

void USSWidgetSlotEntry_WithCursor::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	USSCommonUIFunctionLibrary::HideCursor(this, this);
}
