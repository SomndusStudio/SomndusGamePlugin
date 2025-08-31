// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Setting/Data/SSSettingData_Int.h"

USSSettingData_Int::USSSettingData_Int()
{
	Value = DefaultValue;
}

void USSSettingData_Int::Initialize_Implementation()
{
	Super::Initialize_Implementation();

	Value = DefaultValue;
}

void USSSettingData_Int::SetValue(int32 InValue)
{
	int32 Clamped = FMath::Clamp(InValue, MinValue, MaxValue);
	if (Value != Clamped)
	{
		Value = Clamped;
		NotifyValueChange();
	}
}

FText USSSettingData_Int::GetFormattedText_Implementation()
{
	return FText::AsNumber(Value);
}