// ©2025 Schartier Isaac. All rights reserved.


#include "UI/SSDigitImageWidget.h"

#include "Materials/MaterialInstanceDynamic.h"

void USSDigitImageWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	 
	SetDigit(CurrentDigit);
}
	 
void USSDigitImageWidget::SetDigit(int32 Digit)
{
	int32 ClampedDigit = FMath::Clamp(Digit, 0, 9);
	CurrentDigit = ClampedDigit;
	
	if (auto* DynamicMaterialInstance = GetDynamicMaterial())
	{
		DynamicMaterialInstance->SetScalarParameterValue("Digit", static_cast<float>(ClampedDigit));
	}
}