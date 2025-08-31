/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Setting/SSSettingWidgetFieldBase.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"

FReply USSSettingWidgetFieldBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem();
	if (InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* PrimaryFocus = GetPrimaryGamepadFocusWidget())
		{
			TSharedPtr<SWidget> WidgetToFocus = PrimaryFocus->GetCachedWidget();
			if (WidgetToFocus.IsValid())
			{
				return FReply::Handled().SetUserFocus(WidgetToFocus.ToSharedRef(), InFocusEvent.GetCause());
			}
		}
	}

	return FReply::Unhandled();
}

void USSSettingWidgetFieldBase::SetData_Implementation(USSSettingDataObject* DataObject)
{
	ISSSettingWidgetEntry::SetData_Implementation(DataObject);

	Data = DataObject;
}
