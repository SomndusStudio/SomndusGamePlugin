// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSSubUserSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "UObject/Object.h"
#include "SSGamepadUserSettings.generated.h"

/**
 * Defines a range of sensitivity levels for gamepad settings.
 * Each enumerator represents a specific sensitivity category, ordered progressively
 * from the slowest to the fastest, including descriptive categories for fine-tuning.
 * Includes hidden states for invalid and maximum values.
 */
UENUM(BlueprintType)
enum class ESSGamepadSensitivity : uint8
{
	Invalid = 0		UMETA(Hidden),

	Slow			UMETA(DisplayName = "01 - Slow"),
	SlowPlus		UMETA(DisplayName = "02 - Slow+"),
	SlowPlusPlus	UMETA(DisplayName = "03 - Slow++"),
	Normal			UMETA(DisplayName = "04 - Normal"),
	NormalPlus		UMETA(DisplayName = "05 - Normal+"),
	NormalPlusPlus	UMETA(DisplayName = "06 - Normal++"),
	Fast			UMETA(DisplayName = "07 - Fast"),
	FastPlus		UMETA(DisplayName = "08 - Fast+"),
	FastPlusPlus	UMETA(DisplayName = "09 - Fast++"),
	Insane			UMETA(DisplayName = "10 - Insane"),

	MAX				UMETA(Hidden),
};

/**
 * Represents user-specific settings for configuring gamepad functionality.
 * Handles various aspects of gamepad input configuration such as sensitivity, dead zones,
 * vibration, and overrides for input type.
 */
UCLASS(config=GameUserSettings, configdonotcheckdefaults, DisplayName="Gamepad User Settings", Category="SS|User Settings")
class SOMNDUSGAME_API USSGamepadUserSettings : public USSSubUserSettings
{
	GENERATED_BODY()

public:
	virtual void SetToDefaults() override;
	
	////////////////////////////////////////////////////////
	// Gamepad Sensitivity
private:
	
	/** Multiplier applied while Aiming down sights. */
	UPROPERTY()
	double TargetingMultiplier;

	/** If true then the vertical look axis should be inverted */
	UPROPERTY()
	bool bInvertVerticalAxis;

	/** If true then the horizontal look axis should be inverted */
	UPROPERTY()
	bool bInvertHorizontalAxis;
	
public:
	UFUNCTION(BlueprintPure)
	double GetTargetingMultiplier() const { return TargetingMultiplier; }
	
	UFUNCTION()
	void SetTargetingMultiplier(double NewValue) { ChangeValueAndDirty(TargetingMultiplier, NewValue); ApplyInputSensitivity(); }

	UFUNCTION(BlueprintPure)
	bool GetInvertVerticalAxis() const { return bInvertVerticalAxis; }
	
	UFUNCTION()
	void SetInvertVerticalAxis(bool NewValue) { ChangeValueAndDirty(bInvertVerticalAxis, NewValue); ApplyInputSensitivity(); }

	UFUNCTION(BlueprintPure)
	bool GetInvertHorizontalAxis() const { return bInvertHorizontalAxis; }
	
	UFUNCTION()
	void SetInvertHorizontalAxis(bool NewValue) { ChangeValueAndDirty(bInvertHorizontalAxis, NewValue); ApplyInputSensitivity(); }
	
	////////////////////////////////////////////////////////
	// Gamepad Vibration
public:
	UFUNCTION()
	bool GetForceFeedbackEnabled() const { return bForceFeedbackEnabled; }

	UFUNCTION()
	void SetForceFeedbackEnabled(const bool NewValue) { ChangeValueAndDirty(bForceFeedbackEnabled, NewValue); }
private:
	/** Is force feedback enabled when a controller is being used? */
	UPROPERTY(Config)
	bool bForceFeedbackEnabled = true;

	////////////////////////////////////////////////////////
	// Gamepad Deadzone
private:
	/** Holds the gamepad move stick dead zone value. */
	UPROPERTY(Config)
	float GamepadMoveStickDeadZone;

	/** Holds the gamepad look stick dead zone value. */
	UPROPERTY(Config)
	float GamepadLookStickDeadZone;
	
public:
	/** Getter for gamepad move stick dead zone value. */
	UFUNCTION()
	float GetGamepadMoveStickDeadZone() const { return GamepadMoveStickDeadZone; }

	/** Setter for gamepad move stick dead zone value. */
	UFUNCTION()
	void SetGamepadMoveStickDeadZone(const float NewValue) { ChangeValueAndDirty(GamepadMoveStickDeadZone, NewValue); }

	/** Getter for gamepad look stick dead zone value. */
	UFUNCTION()
	float GetGamepadLookStickDeadZone() const { return GamepadLookStickDeadZone; }

	/** Setter for gamepad look stick dead zone value. */
	UFUNCTION()
	void SetGamepadLookStickDeadZone(const float NewValue) { ChangeValueAndDirty(GamepadLookStickDeadZone, NewValue); }
	
	////////////////////////////////////////////////////////
	// Gamepad Sensitivity
private:
	UPROPERTY(Config)
	ESSGamepadSensitivity GamepadLookSensitivityPreset = ESSGamepadSensitivity::Normal;

	UPROPERTY(Config)
	ESSGamepadSensitivity GamepadTargetingSensitivityPreset = ESSGamepadSensitivity::Normal;

public:
	UFUNCTION()
	ESSGamepadSensitivity GetLookSensitivityPreset() const { return GamepadLookSensitivityPreset; }
	UFUNCTION()
	void SetLookSensitivityPreset(ESSGamepadSensitivity NewValue) { ChangeValueAndDirty(GamepadLookSensitivityPreset, NewValue); ApplyInputSensitivity(); }

	UFUNCTION()
	ESSGamepadSensitivity GetTargetingSensitivityPreset() const { return GamepadTargetingSensitivityPreset; }
	UFUNCTION()
	void SetTargetingSensitivityPreset(ESSGamepadSensitivity NewValue) { ChangeValueAndDirty(GamepadTargetingSensitivityPreset, NewValue); ApplyInputSensitivity(); }

	void ApplyInputSensitivity();

	////////////////////////////////////////////////////////
	// Gamepad Override Input Icon
	UPROPERTY(Config)
	bool bOverrideGamepadInputType = false;

	void ApplyOverriddenInputType();
	
	UFUNCTION()
	bool GetOverrideGamepadInputType() const { return bOverrideGamepadInputType; }

	UFUNCTION()
	void SetOverrideGamepadInputType(bool NewValue) { ChangeValueAndDirty(bOverrideGamepadInputType, NewValue); ApplyOverriddenInputType(); }

	UPROPERTY(Config)
	int32 GamepadInputTypeIndex = 0;
	
	UFUNCTION()
	int32 GetGamepadInputTypeIndex() const { return GamepadInputTypeIndex; }

	UFUNCTION()
	void SetGamepadInputTypeIndex(int32 NewValue) { ChangeValueAndDirty(GamepadInputTypeIndex, NewValue); ApplyOverriddenInputType(); }

public:
	UPROPERTY(BlueprintAssignable)
	FOnGameUserSettingsUINeedsUpdate OnGamepadInputNeedsUpdate;

};
