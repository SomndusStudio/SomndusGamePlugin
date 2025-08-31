// ©2025 Schartier Isaac. All rights reserved.


#include "UI/Input/SSUIGamepadStatics.h"

#include "Components/ScrollBox.h"
#include "Input/SSInputStaticsLibrary.h"

bool USSUIGamepadStatics::ScrollBox_GamepadScroll(const FAnalogInputEvent& AnalogInputEvent, UScrollBox* ScrollBox, float Sensitivity)
{
	float ScrollValue = 0.f;

	// Interpret the Y axis of the right stick, applying a deadzone of 0.1
	// If input is below deadzone or not from the correct axis, early out
	if (!USSInputStaticsLibrary::InterpretRightAnalogDZ(AnalogInputEvent, false, Sensitivity, 0.1f, ScrollValue))
	{
		return false;
	}

	// Calculate the new scroll offset, subtracting the input value
	float NewScrollOffset = ScrollBox->GetScrollOffset() - ScrollValue;

	// Clamp the new offset between 0 and the max scrollable range
	NewScrollOffset = FMath::Clamp(NewScrollOffset, 0.f, ScrollBox->GetScrollOffsetOfEnd());
	
	// Apply the new scroll offset
	ScrollBox->SetScrollOffset(NewScrollOffset);
	
	return true;
}
