// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/SSSubUserSettings.h"
#include "Engine/DeveloperSettings.h"
#include "SSAudioUserSettings.generated.h"

class USoundControlBus;
class USoundControlBusMix;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSSOnAudioSettingNeedsUpdateSignature, int32, Index);

/**
 * Audio user settings class that manages and persists user audio settings such as volume levels
 * and audio quality preferences. Extends USSSubUserSettings.
 * This class allows for the customization of user audio preferences, including global volume,
 * sound effects, music, voice, and UI volume. It also handles enabling or disabling High Dynamic
 * Range Audio (HDR Audio) mode and supports loading user control bus mixes for advanced audio
 * control.
 */
UCLASS(config=GameUserSettings, configdonotcheckdefaults, DisplayName="Audio User Settings", Category="SS|User Settings")
class SOMNDUSGAME_API USSAudioUserSettings : public USSSubUserSettings
{
	GENERATED_BODY()

public:
	virtual void SetToDefaults() override;
	
	void LoadUserControlBusMix();

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<USoundControlBus>> ControlBusMap;

	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> ControlBusMix = nullptr;

private:
	UPROPERTY(Transient)
	bool bSoundControlBusMixLoaded;
	
	UPROPERTY(Config)
	float GlobalVolume;

	UPROPERTY(Config)
	float SFXVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float VoiceVolume;

	UPROPERTY(Config)
	float UIVolume;

public:
	void CommonSetVolume(float Value, FName Key);

	void SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume);
	
	UFUNCTION(BlueprintPure, Category = "Settings|Sound")
	float GetGlobalVolume() const { return GlobalVolume; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Sound")
	void SetGlobalVolume(float Value);

	UFUNCTION(BlueprintPure, Category = "Settings|Sound")
	float GetSFXVolume() const { return SFXVolume;}

	UFUNCTION(BlueprintCallable, Category = "Settings|Sound")
	void SetSFXVolume(float Value);

	UFUNCTION(BlueprintPure, Category = "Settings|Sound")
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Sound")
	void SetMusicVolume(float Value);

	UFUNCTION(BlueprintPure, Category = "Settings|Sound")
	float GetVoiceVolume() const { return VoiceVolume; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Sound")
	void SetVoiceVolume(float Value);

	UFUNCTION(BlueprintPure, Category = "Settings|Sound")
	float GetUIVolume() const {	return UIVolume; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Sound")
	void SetUIVolume(float Value);

	////////////////////////////////////////////////////////
	// Audio Quality
private:
	
	/** Whether to use High Dynamic Range Audio mode (HDR Audio) **/
	UPROPERTY(config)
	bool bUseHDRAudioMode;
	
public:
	/** Returns if we're using High Dynamic Range Audio mode (HDR Audio) **/
	UFUNCTION()
	bool IsHDRAudioModeEnabled() const { return bUseHDRAudioMode; }

	/** Enables or disables High Dynamic Range Audio mode (HDR Audio) */
	UFUNCTION()
	void SetHDRAudioModeEnabled(bool bEnabled);

	UPROPERTY(BlueprintAssignable)
	FOnGameUserSettingsUINeedsUpdate OnAudioModeNeedsUpdate;

	UPROPERTY(BlueprintAssignable)
	FSSOnAudioSettingNeedsUpdateSignature OnAudioNeedsUpdate;

};
