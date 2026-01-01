/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSGameInputActionWrapperWidget.h"

#include "CommonTextBlock.h"
#include "Input/SSGameInputActionWidget.h"

void USSGameInputActionWrapperWidget::SetInputAction(UInputAction* InInputAction)
{
	PendingInputAction = InInputAction;

	if (IsValid(InputActionWidget))
	{
		InputActionWidget->SetInputAction(InInputAction);
	}
}

void USSGameInputActionWrapperWidget::SetDisplayName(FText DisplayText)
{
	DisplayNameTextBlock->SetText(DisplayText);
}

USSGameInputActionWidget* USSGameInputActionWrapperWidget::GetInputActionWidget() const
{
	return InputActionWidget;
}
