// ©2025 Schartier Isaac. All rights reserved.


#include "UI/SSNumberDisplayWidget.h"

#include "Components/HorizontalBox.h"

void USSNumberDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// Set the default preview number in editor and at runtime construction
	SetNumber(DefaultPreviewNumber);
}

void USSNumberDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	 
	if (DigitBox)
	{
		DigitBox->ClearChildren();
		DigitWidgets.Empty();
	}
}
	 
void USSNumberDisplayWidget::SetNumber(int32 Number)
{
	UpdateDigits(Number);
}

void USSNumberDisplayWidget::UpdateDigits(int32 Number)
{
	if (!DigitBox || !DigitWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("DigitBox or DigitWidgetClass not assigned in NumberDisplayWidget!"));
		return;
	}
	 
	FString NumberStr = FString::FromInt(Number);
	int32 NumDigits = NumberStr.Len();
	 
	// Create new digits if needed
	while (DigitWidgets.Num() < NumDigits)
	{
		USSDigitImageWidget* NewDigit = NewObject<USSDigitImageWidget>(this, DigitWidgetClass);
		DigitBox->AddChildToHorizontalBox(NewDigit);
		DigitWidgets.Add(NewDigit);
	}
	 
	// Update visible digits and set digits
	for (int32 i = 0; i < NumDigits; ++i)
	{
		int32 DigitValue = NumberStr[i] - '0';
		USSDigitImageWidget* DigitWidget = DigitWidgets[i];
	 
		DigitWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DigitWidget->SetDigit(DigitValue);
	}
	 
	// Hide excess digit widgets instead of destroying
	for (int32 i = NumDigits; i < DigitWidgets.Num(); ++i)
	{
		DigitWidgets[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}
