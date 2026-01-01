/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Setting/Widget/SSSettingWidgetField_Numeric.h"

#include "UI/Setting/Data/SSSettingData_Int.h"

void USSSettingWidgetField_Numeric::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Left)
		Button_Left->OnClicked().AddUObject(this, &USSSettingWidgetField_Numeric::OnLeftPressed);

	if (Button_Right)
		Button_Right->OnClicked().AddUObject(this, &USSSettingWidgetField_Numeric::OnRightPressed);

	RefreshDisplay(Data);
}

void USSSettingWidgetField_Numeric::SetData_Implementation(USSSettingDataObject* DataObject)
{
	DataInt = Cast<USSSettingData_Int>(DataObject);
	Data = DataObject;

	if (DataInt)
	{
		DataInt->OnValueChange.AddDynamic(this, &USSSettingWidgetField_Numeric::RefreshDisplay);
		RefreshDisplay(Data);
	}
}

void USSSettingWidgetField_Numeric::OnLeftPressed()
{
	if (DataInt)
	{
		DataInt->SetValue(DataInt->GetValue() - DataInt->Step);
		OnValueChanged(DataInt->GetValue());
	}
}

void USSSettingWidgetField_Numeric::OnRightPressed()
{
	if (DataInt)
	{
		DataInt->SetValue(DataInt->GetValue() + DataInt->Step);
		OnValueChanged(DataInt->GetValue());
	}
}

void USSSettingWidgetField_Numeric::RefreshDisplay(USSSettingDataObject* DataObject)
{
	if (Text_SettingValue && DataInt)
	{
		Text_SettingValue->SetText(DataInt->GetFormattedText());
	}
}