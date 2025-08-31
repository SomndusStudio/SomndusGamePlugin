// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSUIGamepadStatics.generated.h"

class UScrollBox;

/**
 * Static utility functions for enhancing UI and widget interaction with gamepads.
 *
 * Provides helpers for handling gamepad analog inputs to drive UMG elements such as ScrollBoxes.
 * All functions are intended to be lightweight, Blueprint-callable, and gamepad-friendly.
 */
UCLASS()
class SOMNDUSGAME_API USSUIGamepadStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Scrolls a UMG ScrollBox vertically using the Y axis of the right analog stick.
	 *
	 * This function processes an analog input event and adjusts the scroll offset of the given ScrollBox
	 * if the input is above a default deadzone.
	 *
	 * @param AnalogInputEvent The analog input event to interpret (expected: Gamepad Right Y).
	 * @param ScrollBox The target ScrollBox to scroll.
	 * @param Sensitivity The multiplier to apply to the analog input value.
	 * @return true if the scroll was adjusted, false if input was below the deadzone or not relevant.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI|Gamepad")
	static bool ScrollBox_GamepadScroll(const FAnalogInputEvent& AnalogInputEvent, UScrollBox* ScrollBox, float Sensitivity = 30.f);
};
