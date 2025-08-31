// ©2025 Schartier Isaac. All rights reserved.


#include "Settings/SSSubUserSettings.h"

#include "SSLog.h"

void USSSubUserSettings::SetToDefaults()
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] SetToDefaults"), *GetNameSafe(this));
}

void USSSubUserSettings::LoadSettings()
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] LoadSettings"), *GetNameSafe(this));
	LoadConfig();
}

void USSSubUserSettings::SaveSettings()
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] SaveSettings"), *GetNameSafe(this));
	SaveConfig();
}
