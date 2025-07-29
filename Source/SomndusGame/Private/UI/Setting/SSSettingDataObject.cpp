// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Setting/SSSettingDataObject.h"

void USSSettingDataObject::Initialize_Implementation()
{
}

void USSSettingDataObject::NotifyDefaultValueChange()
{
	OnDefaultValueChange.Broadcast(this);
}

void USSSettingDataObject::NotifyValueChange()
{
	OnValueChange.Broadcast(this);
}

FText USSSettingDataObject::GetFormattedText_Implementation()
{
	return DisplayLabel;
}
