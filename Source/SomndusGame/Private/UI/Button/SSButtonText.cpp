/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#include "UI/Button/SSButtonText.h"

void USSButtonText::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetButtonText(ButtonText);
}

void USSButtonText::SetButtonText(const FText& InText)
{
	ButtonText = InText;
	RefreshButtonText();
}

void USSButtonText::RefreshButtonText()
{
	UpdateButtonText(ButtonText);	
}
