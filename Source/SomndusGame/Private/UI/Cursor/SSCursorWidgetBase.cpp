/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Cursor/SSCursorWidgetBase.h"

#include "UI/SSCommonUIFunctionLibrary.h"
#include "UI/Cursor/SSCursorHandler.h"

void USSCursorWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Invoker)
	{
		if (Invoker->Implements<USSCursorHandler>())
		{
			auto CursorInfo = ISSCursorHandler::Execute_GetCursorRequestInfo(Invoker);
			Invoker->SetPositionInViewport(CursorInfo.Position, true);
		}
	}
}

void USSCursorWidgetBase::HandleWidgetDestruct(UUserWidget* UserWidget)
{
	if (UserWidget)
	{
		USSCommonUIFunctionLibrary::HideCursor(UserWidget, UserWidget);
	}
}

void USSCursorWidgetBase::NativeUpdateCursorInfo(UUserWidget* InInvoker)
{
	// unbind previous
	if (InInvoker){
		InInvoker->OnNativeDestruct.Remove(DestructDelegateHandle);
	}

	Invoker = InInvoker;
	
	DestructDelegateHandle = Invoker->OnNativeDestruct.AddUObject(this, &USSCursorWidgetBase::HandleWidgetDestruct);
}


void USSCursorWidgetBase::UpdateCursorInfo_Implementation(FSSCursorRequestInfo CursorRequestInfo)
{
}