// Fill out your copyright notice in the Description page of Project Settings.


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
