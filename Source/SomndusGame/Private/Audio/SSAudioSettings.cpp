/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Audio/SSAudioSettings.h"

#define LOCTEXT_NAMESPACE "SomndusGame"

USSAudioSettings::USSAudioSettings(const FObjectInitializer& Initializer)
{
	SectionName = TEXT("Somndus Game - Audio");
}

#if WITH_EDITOR
FText USSAudioSettings::GetSectionText() const
{
	return LOCTEXT("SettingsDisplayName", "Somndus Game - Audio");

}
#endif

const USSAudioSettings* USSAudioSettings::Get()
{
	return GetDefault<USSAudioSettings>();
}

#undef LOCTEXT_NAMESPACE