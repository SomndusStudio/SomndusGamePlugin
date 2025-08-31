// ©2025 Schartier Isaac. All rights reserved.


#include "Settings/SSGamepadUserSettings.h"

#include "Core/SSCoreGlobals.h"

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
