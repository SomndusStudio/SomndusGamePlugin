/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonInputSubsystem.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Input/Events.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSInputStaticsLibrary.generated.h"

class USSInputLocalPlayerSubsystem;

/**
 * Retrieves the display name of the key assigned to an Enhanced Input Action.
 *
 * @param CommonInputSubsystem The input subsystem to query.
 * @param InputAction The input action to retrieve the key name for.
 * @return The display text representing the key assigned to the input action.
 */
UCLASS()
class SOMNDUSGAME_API USSInputStaticsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static ULocalPlayer* GetLocalPlayer(const UObject* WorldContextObject);
	static const UCommonInputSubsystem* GetCommonInputSubsystem(const UObject* WorldContextObject);
	static USSInputLocalPlayerSubsystem* GetInputLocalPlayerSubsystem(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static FText GetKeyNameForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	static FText GetKeyNameForInputActionType(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction, bool bGamepad = false);

	UFUNCTION(BlueprintCallable)
	static bool IsKeyInInputAction(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction, const UInputAction* OtherInputAction);

	static FKey GetFirstKeyForInputType(const UCommonInputSubsystem* CommonInputSubsystem, ECommonInputType InputType, const TArray<FDataTableRowHandle>& InputActions);

	UFUNCTION(BlueprintCallable)
	static FKey GetFirstKeyForInputType(const USSInputLocalPlayerSubsystem* SSInputLocalPlayer, ECommonInputType InputType, const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	static FSlateBrush GetIconForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	static FSlateBrush GetIconForInputActionType(const UCommonInputSubsystem* CommonInputSubsystem, const UInputAction* InputAction, bool bGamepad = false);

	UFUNCTION(BlueprintCallable)
	static FSlateBrush GetIconForKey(const UCommonInputSubsystem* CommonInputSubsystem, const FKey Key);

	UFUNCTION(BlueprintCallable, Category = "SS|Input", meta=(WorldContext="WorldContextObject"))
	static UTexture2D* GetGamepadInputIconFromKey(const UObject* WorldContextObject, const FKey& Key, const FName& GamepadName);

	UFUNCTION(BlueprintCallable, Category = "SS|Input", meta=(WorldContext="WorldContextObject"))
	static UTexture2D* GetInputIconFromKey(const UObject* WorldContextObject, const FKey& Key);

	UFUNCTION(BlueprintPure, Category = "Input", meta=(WorldContext="WorldContextObject"))
	static UTexture2D* GetInputIconFromInputTypeKey(const UObject* WorldContextObject, ECommonInputType InputType, const FKey& Key);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(WorldContext="WorldContextObject"))
	static void SetGameSuspendInput(const UObject* WorldContextObject, FGameplayTag Tag, bool bSuspend = true);

	/**
	 * Interprets a right analog stick input event (X or Y axis) and applies a sensitivity multiplier.
	 *
	 * @param AnalogInputEvent The analog input event to interpret.
	 * @param InRight Whether to interpret the X axis (true) or Y axis (false) of the right stick.
	 * @param Sensibility Multiplier to apply to the analog input value.
	 * @param OutValue The resulting scaled analog value if the key matches.
	 * @return true if the input event matches the selected axis and OutValue is set, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI")
	static bool InterpretRightAnalog(const FAnalogInputEvent& AnalogInputEvent, bool InRight, float Sensibility, float& OutValue);

	/**
	 * Interprets a right analog stick input event (X or Y axis) with deadzone handling and sensitivity scaling.
	 *
	 * @param AnalogInputEvent The analog input event to interpret.
	 * @param InRight Whether to interpret the X axis (true) or Y axis (false) of the right stick.
	 * @param Sensibility Multiplier to apply to the analog input value.
	 * @param DeadZone Threshold below which the input is considered zero.
	 * @param OutValue The resulting scaled analog value if valid.
	 * @return true if the input is above the deadzone and OutValue is set, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI")
	static bool InterpretRightAnalogDZ(const FAnalogInputEvent& AnalogInputEvent, bool InRight, float Sensibility, float DeadZone, float& OutValue);

	/**
	 * Interprets a left analog stick movement using X and Y axes and applies sensitivity scaling.
	 * This version applies no deadzone.
	 *
	 * @param AnalogInputEvent The analog input event to interpret.
	 * @param Sensibility Multiplier to apply to the analog input values.
	 * @param OutValue The resulting 2D vector representing stick direction and magnitude.
	 * @return true if the event is for the left stick and OutValue is set, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI")
	static bool InterpretLeftAnalogMove(const FAnalogInputEvent& AnalogInputEvent, float Sensibility, FVector2D& OutValue);
	
	/**
	 * Interprets a left analog stick movement using X and Y axes with deadzone handling and sensitivity scaling.
	 *
	 * @param AnalogInputEvent The analog input event to interpret.
	 * @param Sensibility Multiplier to apply to the analog input values.
	 * @param DeadZone Threshold below which input is ignored.
	 * @param OutValue The resulting 2D vector representing stick direction and magnitude.
	 * @return true if the input is above the deadzone and OutValue is set, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI")
	static bool InterpretLeftAnalogMoveDZ(const FAnalogInputEvent& AnalogInputEvent, float Sensibility, float DeadZone, FVector2D& OutValue);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "SS|UI")
	static bool InterpretRightAnalogMove(const FAnalogInputEvent& AnalogInputEvent, float Sensibility, FVector2D& OutValue);
	
	static bool InterpretAnalogMoveDZ(const FAnalogInputEvent& AnalogInputEvent, bool bRight, float Sensibility, float DeadZone, FVector2D& OutValue);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS|Input", meta=(WorldContext="WidgetContextObject"))
	static bool IsPlayGamepadFromWidget(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="SS|Input", DisplayName="IsPlayGamepadWidget (Branch)", meta=(WorldContext="WidgetContextObject", ExpandBoolAsExecs = "ReturnValue"))
	static bool BranchIsPlayGamepadFromWidget(const UUserWidget* WidgetContextObject);
	
	UFUNCTION(BlueprintPure, Category="SS|Input", meta=(WorldContext="WorldContextObject"))
	static bool IsPlayGamepad(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="SS|Input", DisplayName="IsPlayGamepad", meta=(WorldContext="WorldContextObject", ExpandBoolAsExecs = "ReturnValue"))
	static bool BranchIsPlayGamepad(const UObject* WorldContextObject);
};
