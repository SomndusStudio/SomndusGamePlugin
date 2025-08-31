/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Cursor/SSCursorWidgetCapture.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "UI/SSCommonUIFunctionLibrary.h"

FSSCursorRequestInfo USSCursorWidgetCapture::GetCursorRequestInfo()
{
	FSSCursorRequestInfo Result = CursorInfo;
	
	auto AbsoluteLocation = USSCommonUIFunctionLibrary::GetAbsolutePosition(this);
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	
	USlateBlueprintLibrary::AbsoluteToViewport(this, AbsoluteLocation, PixelPosition, ViewportPosition);

	Result.Position = PixelPosition;

	return Result;
}
