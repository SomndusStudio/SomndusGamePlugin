// © 2024 Schartier Isaac. All rights reserved.


#include "Input/SSInputPlatformWidgetSet.h"

FSSInputPlatformWidget USSInputPlatformWidgetSet::GetSettingFromName(FName GamepadName)
{
	for (const auto& InputPlatformWidgetSetting : InputPlatformWidgetSettings)
	{
		if (InputPlatformWidgetSetting.GamepadName == GamepadName)
		{
			return InputPlatformWidgetSetting;
		}
	}

	return FSSInputPlatformWidget();
}

bool USSInputPlatformWidgetSet::GetValidSettingFromName(FName GamepadName,
	FSSInputPlatformWidget& InputPlatformWidgetSetting)
{
	for (const auto& Value : InputPlatformWidgetSettings)
	{
		if (Value.GamepadName == GamepadName)
		{
			InputPlatformWidgetSetting = Value;
			return true;
		}
	}
	return false;
}
