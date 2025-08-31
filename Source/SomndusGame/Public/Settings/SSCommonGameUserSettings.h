// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSCultureUserSettings.h"
#include "SSGamepadUserSettings.h"
#include "SSGameUserSettings.h"
#include "SSMouseKeyboardUserSettings.h"
#include "Audio/SSAudioUserSettings.h"

#include "SSCommonGameUserSettings.generated.h"



#define SS_GET_GAMEPAD_SETTINGS_PATH(FunctionName) \
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({ \
GET_FUNCTION_NAME_STRING_CHECKED(USSLocalPlayer, GetGameUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSCommonGameUserSettings, GetGamepadSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSGamepadUserSettings, FunctionName) \
}))

#define SS_GET_KEYBOARD_SETTINGS_PATH(FunctionName) \
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({ \
GET_FUNCTION_NAME_STRING_CHECKED(USSLocalPlayer, GetGameUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSCommonGameUserSettings, GetMouseKeyboardUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSMouseKeyboardUserSettings, FunctionName) \
}))

#define SS_GET_AUDIO_SETTINGS_PATH(FunctionName) \
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({ \
GET_FUNCTION_NAME_STRING_CHECKED(USSLocalPlayer, GetGameUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSCommonGameUserSettings, GetAudioUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSAudioUserSettings, FunctionName) \
}))

#define SS_GET_CULTURE_SETTINGS_PATH(FunctionName) \
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({ \
GET_FUNCTION_NAME_STRING_CHECKED(USSLocalPlayer, GetGameUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSCommonGameUserSettings, GetCultureUserSettings), \
GET_FUNCTION_NAME_STRING_CHECKED(USSCultureUserSettings, FunctionName) \
}))

/**
 * Represents a common game user settings class that serves as a container for multiple sub-settings.
 * It extends the functionality of USSGameUserSettings by including specific sub-settings for:
 * - Gamepad input
 * - Mouse/Keyboard input
 * - Audio configurations
 * - Culture settings
 */
UCLASS()
class SOMNDUSGAME_API USSCommonGameUserSettings : public USSGameUserSettings
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<USSGamepadUserSettings> GamepadSettings;

	UPROPERTY()
	TObjectPtr<USSMouseKeyboardUserSettings> MouseKeyboardUserSettings;

	UPROPERTY()
	TObjectPtr<USSAudioUserSettings> AudioUserSettings;

	UPROPERTY()
	TObjectPtr<USSCultureUserSettings> CultureUserSettings;
	
	/**
	 * Returns the sub-settings object for gamepad-related input configuration.
	 */
	UFUNCTION(BlueprintCallable, Category = "SS|Settings")
	USSGamepadUserSettings* GetGamepadSettings() const { return GamepadSettings; }

	/**
	 * Returns the sub-settings object for mouse/keyboard-related input configuration.
	 */
	UFUNCTION(BlueprintCallable, Category = "SS|Settings")
	USSMouseKeyboardUserSettings* GetMouseKeyboardUserSettings() const { return MouseKeyboardUserSettings; }

	/**
	 * Returns the sub-settings object for audio-related configuration.
	 */
	UFUNCTION(BlueprintCallable, Category = "SS|Settings")
	USSAudioUserSettings* GetAudioUserSettings() const { return AudioUserSettings; }

	/**
	 * Returns the sub-settings object for culture-related configuration.
	 */
	UFUNCTION(BlueprintCallable, Category = "SS|Settings")
	USSCultureUserSettings* GetCultureUserSettings() const { return CultureUserSettings; }
	
public:
	
	virtual void CheckSubSettings() override;
};
