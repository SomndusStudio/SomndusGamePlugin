// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Setting/Widget/SSSettingWidgetField_Scalar.h"

#include "UI/Setting/SSSettingDataObject.h"
#include "UI/Setting/Data/SSSettingData_Float.h"

void USSSettingWidgetField_Scalar::Refresh()
{
	if (ensure(DataFloat))
	{
		const float Value = DataFloat->GetValueNormalized();

		Slider_SettingValue->SetValue(Value);
		Slider_SettingValue->SetStepSize(DataFloat->GetNormalizedStepSize());
		Text_SettingValue->SetText(Data->GetFormattedText());

		TOptional<double> DefaultValue = DataFloat->DefaultValue;
		OnDefaultValueChanged(DefaultValue.IsSet() ? DefaultValue.GetValue() : -1.0);

		OnValueChanged(Value);
	}
}

void USSSettingWidgetField_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Slider_SettingValue->OnValueChanged.AddDynamic(this, &ThisClass::HandleSliderValueChanged);

	Slider_SettingValue->OnMouseCaptureEnd.AddDynamic(this, &ThisClass::HandleSliderCaptureEnded);
	Slider_SettingValue->OnControllerCaptureEnd.AddDynamic(this, &ThisClass::HandleSliderCaptureEnded);
}

void USSSettingWidgetField_Scalar::HandleSliderValueChanged(float Value)
{
	TGuardValue<bool> Suspend(bSuspendChangeUpdates, true);
	
	if (ensure(DataFloat))
	{
		DataFloat->SetValue(Value);
		Value = DataFloat->GetValueNormalized();

		Slider_SettingValue->SetValue(Value);
		Text_SettingValue->SetText(DataFloat->GetFormattedText());

		OnValueChanged(Value);
	}
}

void USSSettingWidgetField_Scalar::HandleSliderCaptureEnded()
{
	TGuardValue<bool> Suspend(bSuspendChangeUpdates, true);
}

void USSSettingWidgetField_Scalar::SetData_Implementation(USSSettingDataObject* DataObject)
{
	Super::SetData_Implementation(DataObject);
	DataFloat = Cast<USSSettingData_Float>(Data);
	
	Refresh();
}
