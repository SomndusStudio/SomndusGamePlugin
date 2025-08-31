/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Button/SSButtonText_WithCursor.h"

#include "UI/SSCommonUIFunctionLibrary.h"

FSSCursorRequestInfo USSButtonText_WithCursor::GetCursorRequestInfo_Implementation()
{
	if (HandCursorCapture)
	{
		return HandCursorCapture->GetCursorRequestInfo();
	}
	return ISSCursorHandler::GetCursorRequestInfo_Implementation();
}

void USSButtonText_WithCursor::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	USSCommonUIFunctionLibrary::ShowCursor(this, this);
}

void USSButtonText_WithCursor::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	USSCommonUIFunctionLibrary::HideCursor(this, this);
}
