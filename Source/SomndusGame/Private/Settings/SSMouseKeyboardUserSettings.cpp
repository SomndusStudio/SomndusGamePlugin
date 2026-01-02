/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Settings/SSMouseKeyboardUserSettings.h"

#include "SSLog.h"

void USSMouseKeyboardUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	
	MouseSensitivity = 0.5f;
	bInvertMouse = false;
	bInvertMouseX = false;
	
	MouseSensitivityX = 1.0f;
	MouseSensitivityY = 1.0f;
}

void USSMouseKeyboardUserSettings::ApplyInputSensitivity()
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] ApplyInputSensitivity"), *GetNameSafe(this));
}
