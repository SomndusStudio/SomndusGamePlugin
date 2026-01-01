/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Button/SSButtonGamepadInteraction.h"

#include "CommonUIExtensions.h"

void USSButtonGamepadInteraction::NativeOnHovered()
{
	UpdateTriggeringAction();
	Super::NativeOnHovered();

	if (bHoverPerformWithGamepad)
	{
		if (UCommonUIExtensions::GetOwningPlayerInputType(this) == ECommonInputType::Gamepad)
		{
			NativeOnClicked();
		}
	}
}

void USSButtonGamepadInteraction::NativeOnUnhovered()
{
	ClearTriggeringAction();
	Super::NativeOnUnhovered();
}

void USSButtonGamepadInteraction::SetTriggeringActionIndex(int32 Index)
{
	TriggeringInputActionIndex = Index;
	
	UpdateTriggeringAction();
}

void USSButtonGamepadInteraction::ClearTriggeringAction()
{
	if (ConditionalTriggeringInputActions.IsEmpty()) return;
	
	SetTriggeringInputAction(FDataTableRowHandle());
}

void USSButtonGamepadInteraction::UpdateTriggeringAction_Implementation()
{
	if (ConditionalTriggeringInputActions.IsEmpty()) return;
	
	SetTriggeringInputAction(ConditionalTriggeringInputActions[TriggeringInputActionIndex]);
}