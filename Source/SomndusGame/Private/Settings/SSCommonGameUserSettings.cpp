// ©2025 Schartier Isaac. All rights reserved.


#include "Settings/SSCommonGameUserSettings.h"

void USSCommonGameUserSettings::CheckSubSettings()
{
	Super::CheckSubSettings();
	
	// Sub settings
	EnsureSubSettings(GamepadSettings);
	EnsureSubSettings(MouseKeyboardUserSettings);
	EnsureSubSettings(AudioUserSettings);
	EnsureSubSettings(CultureUserSettings);
}