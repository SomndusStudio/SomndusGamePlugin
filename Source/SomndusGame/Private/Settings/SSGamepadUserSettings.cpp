/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Settings/SSGamepadUserSettings.h"

#include "SSLog.h"

void USSGamepadUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	
	TargetingMultiplier = 0.5f;
	bInvertVerticalAxis = false;
	bInvertHorizontalAxis = false;
}

void USSGamepadUserSettings::ApplyInputSensitivity()
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] ApplyInputSensitivity"), *GetNameSafe(this));
}

void USSGamepadUserSettings::ApplyOverriddenInputType()
{
	OnGamepadInputNeedsUpdate.Broadcast();
}
