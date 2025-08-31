// ©2025 Schartier Isaac. All rights reserved.


#include "Settings/SSMouseKeyboardUserSettings.h"

#include "SSLog.h"

void USSMouseKeyboardUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	
	MouseSensitivity = 0.5f;
	bInvertMouse = false;
	
	MouseSensitivityX = 1.0f;
	MouseSensitivityY = 1.0f;
}

void USSMouseKeyboardUserSettings::ApplyInputSensitivity()
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] ApplyInputSensitivity"), *GetNameSafe(this));
}
