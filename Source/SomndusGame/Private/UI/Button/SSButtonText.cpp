/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Button/SSButtonText.h"

void USSButtonText::SetButtonText(const FText& InText)
{
	ButtonText = InText;
	RefreshButtonText();
}

void USSButtonText::RefreshButtonText()
{
	UpdateButtonText(ButtonText);	
}
