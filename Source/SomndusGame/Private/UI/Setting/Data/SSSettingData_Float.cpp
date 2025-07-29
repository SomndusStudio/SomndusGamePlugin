// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Setting/Data/SSSettingData_Float.h"

#define LOCTEXT_NAMESPACE "SomndusGame"

static FText PercentFormat = LOCTEXT("PercentFormat", "{0}%");

USSSettingData_Float::USSSettingData_Float()
{
	Value = DefaultValue;
}

float USSSettingData_Float::GetValue()
{
	return Value;
}

void USSSettingData_Float::SetValue(float InValue)
{
	Value = InValue;
}

TRange<double> USSSettingData_Float::GetSourceRange() const
{
	return TRange<double>(MinValue, MaxValue);
}

double USSSettingData_Float::GetSourceStep() const
{
	return 0.01f;
}

FText USSSettingData_Float::GetFormattedText_Implementation()
{
	const double SourceValue = GetValue() * 100;
	
	return FText::Format(PercentFormat, (int32)FMath::RoundHalfFromZero(SourceValue));
}

float USSSettingData_Float::GetNormalizedStepSize()
{
	TRange<double> SourceRange = GetSourceRange();
	return GetSourceStep() / FMath::Abs(SourceRange.GetUpperBoundValue() - SourceRange.GetLowerBoundValue());
}

float USSSettingData_Float::GetValueNormalized()
{
	return Value;
}
#undef LOCTEXT_NAMESPACE
