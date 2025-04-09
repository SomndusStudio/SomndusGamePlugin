// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Setting/Data/SSSettingData_Options.h"

int32 USSSettingData_Options::GetValue()
{
	return IndexValue;
}

void USSSettingData_Options::SetIndexValue(int32 InValue)
{
	IndexValue = InValue;
}
