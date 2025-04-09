// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Setting/SSSettingWidgetThemeDataAsset.h"

FSSWidgetSettingThemeInfo& USSSettingWidgetThemeDataAsset::GetSettingThemeInfo(const FGameplayTag& Tag)
{
	for (auto& SettingThemeInfo : SettingThemeInfos)
	{
		if (SettingThemeInfo.Tag == Tag) return SettingThemeInfo;
	}
	return DefaultSettingThemeInfo;
}
