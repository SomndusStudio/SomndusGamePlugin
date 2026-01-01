/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Audio/SSAudioUserSettings.h"

#include "AudioModulationStatics.h"
#include "Audio/SSAudioSettings.h"
#include "Engine/Engine.h"

void USSAudioUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	bUseHDRAudioMode = false;

	GlobalVolume = 0.5f;
	SFXVolume = 0.5f;
	MusicVolume = 0.5f;
	VoiceVolume = 0.5f;
	UIVolume = 0.5f;
}

void USSAudioUserSettings::LoadUserControlBusMix()
{
	if (GEngine)
	{
		if (const UWorld* World = GEngine->GetCurrentPlayWorld())
		{
			if (const USSAudioSettings* GameAudioSettings = GetDefault<USSAudioSettings>())
			{
				USoundControlBus* OverallControlBus = nullptr;
				USoundControlBus* MusicControlBus = nullptr;
				USoundControlBus* SoundFXControlBus = nullptr;
				USoundControlBus* DialogueControlBus = nullptr;
				USoundControlBus* UIControlBus = nullptr;

				ControlBusMap.Empty();

				if (UObject* ObjPath = GameAudioSettings->OverallVolumeControlBus.TryLoad())
				{
					if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
					{
						OverallControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("Overall"), OverallControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("Overall Control Bus reference missing from Lyra Audio Settings."));
					}
				}

				if (UObject* ObjPath = GameAudioSettings->MusicVolumeControlBus.TryLoad())
				{
					if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
					{
						MusicControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("Music"), MusicControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("Music Control Bus reference missing from Game Audio Settings."));
					}
				}

				if (UObject* ObjPath = GameAudioSettings->SoundFXVolumeControlBus.TryLoad())
				{
					if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
					{
						SoundFXControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("SoundFX"), SoundFXControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("SoundFX Control Bus reference missing from Game Audio Settings."));
					}
				}

				if (UObject* ObjPath = GameAudioSettings->DialogueVolumeControlBus.TryLoad())
				{
					if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
					{
						DialogueControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("Dialogue"), DialogueControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("Dialogue Control Bus reference missing from Game Audio Settings."));
					}
				}

				if (UObject* ObjPath = GameAudioSettings->UIVolumeControlBus.TryLoad())
				{
					if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
					{
						UIControlBus = SoundControlBus;
						ControlBusMap.Add(TEXT("UI"), UIControlBus);
					}
					else
					{
						ensureMsgf(SoundControlBus, TEXT("UI Control Bus reference missing from Game Audio Settings."));
					}
				}

				if (UObject* ObjPath = GameAudioSettings->UserSettingsControlBusMix.TryLoad())
				{
					if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(ObjPath))
					{
						ControlBusMix = SoundControlBusMix;

						const FSoundControlBusMixStage OverallControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, OverallControlBus, GlobalVolume);
						const FSoundControlBusMixStage MusicControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, MusicControlBus, MusicVolume);
						const FSoundControlBusMixStage SoundFXControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, SoundFXControlBus, SFXVolume);
						const FSoundControlBusMixStage DialogueControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, DialogueControlBus, VoiceVolume);
						const FSoundControlBusMixStage UIControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, UIControlBus, UIVolume);

						TArray<FSoundControlBusMixStage> ControlBusMixStageArray;
						ControlBusMixStageArray.Add(OverallControlBusMixStage);
						ControlBusMixStageArray.Add(MusicControlBusMixStage);
						ControlBusMixStageArray.Add(SoundFXControlBusMixStage);
						ControlBusMixStageArray.Add(DialogueControlBusMixStage);
						ControlBusMixStageArray.Add(UIControlBusMixStage);

						UAudioModulationStatics::UpdateMix(World, ControlBusMix, ControlBusMixStageArray);

						bSoundControlBusMixLoaded = true;
					}
					else
					{
						ensureMsgf(SoundControlBusMix, TEXT("User Settings Control Bus Mix reference missing from Game Audio Settings."));
					}
				}
			}
		}
	}
}

void USSAudioUserSettings::CommonSetVolume(float Value, FName Key)
{
	// Check to see if references to the control buses and control bus mixes have been loaded yet
	// Will likely need to be loaded if this function is the first time a setter has been called from the UI
	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	// Ensure it's been loaded before continuing
	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	// Locate the locally cached bus and set the volume on it
	if (TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(Key))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, Value);
		}
	}
}

void USSAudioUserSettings::SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume)
{
	// Check to see if references to the control buses and control bus mixes have been loaded yet
	// Will likely need to be loaded if this function is the first time a setter has been called
	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	// Ensure it's been loaded before continuing
	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	// Assuming everything has been loaded correctly, we retrieve the world and use AudioModulationStatics to update the Control Bus Volume values and
	// apply the settings to the cached User Control Bus Mix
	if (GEngine && InSoundControlBus && bSoundControlBusMixLoaded)
	{
		if (const UWorld* AudioWorld = GEngine->GetCurrentPlayWorld())
		{
			ensureMsgf(ControlBusMix, TEXT("Control Bus Mix failed to load."));

			// Create and set the Control Bus Mix Stage Parameters
			FSoundControlBusMixStage UpdatedControlBusMixStage;
			UpdatedControlBusMixStage.Bus = InSoundControlBus;
			UpdatedControlBusMixStage.Value.TargetValue = InVolume;
			UpdatedControlBusMixStage.Value.AttackTime = 0.01f;
			UpdatedControlBusMixStage.Value.ReleaseTime = 0.01f;

			// Add the Control Bus Mix Stage to an Array as the UpdateMix function requires
			TArray<FSoundControlBusMixStage> UpdatedMixStageArray;
			UpdatedMixStageArray.Add(UpdatedControlBusMixStage);

			// Modify the matching bus Mix Stage parameters on the User Control Bus Mix
			UAudioModulationStatics::UpdateMix(AudioWorld, ControlBusMix, UpdatedMixStageArray);
		}
	}
}

void USSAudioUserSettings::SetGlobalVolume(float Value)
{
	GlobalVolume = Value;
	CommonSetVolume(GlobalVolume, "Overall");
	OnAudioNeedsUpdate.Broadcast(0);
}

void USSAudioUserSettings::SetSFXVolume(float Value)
{
	SFXVolume = Value;
	CommonSetVolume(SFXVolume, "SoundFX");
	OnAudioNeedsUpdate.Broadcast(2);
}

void USSAudioUserSettings::SetMusicVolume(float Value)
{
	MusicVolume = Value;
	CommonSetVolume(MusicVolume, "Music");
	OnAudioNeedsUpdate.Broadcast(1);
}

void USSAudioUserSettings::SetVoiceVolume(float Value)
{
	VoiceVolume = Value;
	CommonSetVolume(VoiceVolume, "Dialogue");
	OnAudioNeedsUpdate.Broadcast(3);
}

void USSAudioUserSettings::SetUIVolume(float Value)
{
	UIVolume = Value;
	CommonSetVolume(UIVolume, "UI");
	OnAudioNeedsUpdate.Broadcast(4);
}

void USSAudioUserSettings::SetHDRAudioModeEnabled(bool bEnabled)
{
	bUseHDRAudioMode = bEnabled;

	// Notify need change
	OnAudioModeNeedsUpdate.Broadcast();
}
