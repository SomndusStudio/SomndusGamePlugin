// ©2025 Schartier Isaac. All rights reserved.


#include "Settings/SSGameUserSettings.h"

#include "SSLog.h"

void USSGameUserSettings::CheckSubSettings()
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] CheckSubSettings"), *GetNameSafe(this));
}

void USSGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
}

void USSGameUserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	CheckSubSettings();
	
	for (const auto& Setting : SubSettings)
	{
		Setting->LoadSettings();
	}
}

void USSGameUserSettings::SaveSettings()
{
	Super::SaveSettings();

	// Save all registered sub-settings manually
	// Each sub-settings object must implement SaveConfig()
	for (const auto& Setting : SubSettings)
	{
		Setting->SaveSettings();
	}
}
