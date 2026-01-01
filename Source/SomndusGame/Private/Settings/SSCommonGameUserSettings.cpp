/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Settings/SSCommonGameUserSettings.h"

#include "Engine/Engine.h"

USSCommonGameUserSettings* USSCommonGameUserSettings::Get()
{
	return GEngine ? CastChecked<USSCommonGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void USSCommonGameUserSettings::CheckSubSettings()
{
	Super::CheckSubSettings();
	
	// Sub settings
	EnsureSubSettings(GamepadSettings);
	EnsureSubSettings(MouseKeyboardUserSettings);
	EnsureSubSettings(AudioUserSettings);
	EnsureSubSettings(CultureUserSettings);
}
