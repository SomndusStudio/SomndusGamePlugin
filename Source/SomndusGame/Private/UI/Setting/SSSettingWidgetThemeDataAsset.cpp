/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Setting/SSSettingWidgetThemeDataAsset.h"

FSSWidgetSettingThemeInfo& USSSettingWidgetThemeDataAsset::GetSettingThemeInfo(const FGameplayTag& Tag)
{
	for (auto& SettingThemeInfo : SettingThemeInfos)
	{
		if (SettingThemeInfo.Tag == Tag) return SettingThemeInfo;
	}
	return DefaultSettingThemeInfo;
}
