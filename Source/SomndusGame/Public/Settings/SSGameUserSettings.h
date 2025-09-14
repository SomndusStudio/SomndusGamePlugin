// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSLog.h"
#include "SSSubUserSettings.h"
#include "GameFramework/GameUserSettings.h"

#include "SSGameUserSettings.generated.h"

/**
 * Represents a game user settings class that encapsulates base game settings
 * functionality, extended to support sub-settings management.
 *
 * This class allows for the creation and management of sub-settings objects,
 * enabling easier configuration via modular sub-components. It includes methods
 * for managing, loading, and saving both the main and sub-settings.
 */
UCLASS()
class SOMNDUSGAME_API USSGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	
	/**
	 * Creates and registers a sub-settings object of the given type.
	 * The object is automatically:
	 * - Instantiated with this object as its outer
	 * - Loaded from its config section (via LoadConfig)
	 * - Added to the internal list for later saving
	 *
	 * @tparam T The type of sub-settings object to create (must derive from USSSubUserSettings)
	 * @return A pointer to the newly created sub-settings object
	 */
	template <typename T>
	T* CreateSubSettings()
	{
		static_assert(TIsDerivedFrom<T, USSSubUserSettings>::IsDerived, "T must inherit from USSSubUserSettings");

		UObject* Outer = HasAnyFlags(RF_ClassDefaultObject) ? (UObject*) GetTransientPackage() : this;
		
		T* NewSubSettings = NewObject<T>(Outer);

		UE_LOG(LogSomndusGame, Log, TEXT("Creating sub-settings %s"), *T::StaticClass()->GetName());
		
		if (HasAnyFlags(RF_ClassDefaultObject))
		{
			NewSubSettings->AddToRoot();
			UE_LOG(LogSomndusGame, Log, TEXT("Attach sub-settings %s to Root cause RF_ClassDefaultObject"), *T::StaticClass()->GetName());
		}
		
		NewSubSettings->SetToDefaults();
		SubSettings.Add(NewSubSettings);

		
		return NewSubSettings;
	}
	
	/**
	 * Ensures that the given sub-settings object is initialized.
	 *
	 * If the provided pointer is null, this function will create the sub-settings instance
	 * using CreateSubSettings<T>(), register it for config management, and assign it to the given reference.
	 * Otherwise, it simply returns the existing instance.
	 *
	 * Intended to simplify sub-settings initialization inside UGameUserSettings-like classes.
	 *
	 * @tparam T The sub-settings class type (must inherit from USSSubUserSettings)
	 * @param Target Reference to the TObjectPtr that should hold the sub-settings instance
	 * @return A valid pointer to the initialized sub-settings object
	 */
	template<typename T>
	T* EnsureSubSettings(TObjectPtr<T>& Target)
	{
		static_assert(TIsDerivedFrom<T, USSSubUserSettings>::IsDerived, "T must inherit from USSSubUserSettings");

		if (!Target)
		{
			Target = CreateSubSettings<T>();
		}
		return Target;
	}
	
	virtual void CheckSubSettings();
	
protected:

	/** Internal list of all sub-settings created through CreateSubSettings, used for saving */
	UPROPERTY()
	TArray<TObjectPtr<USSSubUserSettings>> SubSettings;
	
	virtual void SetToDefaults() override;
	
	/**
	 * Loads both the base UGameUserSettings and all sub-settings objects.
	 * Sub-settings are recreated and their values loaded from config.
	 */
	virtual void LoadSettings(bool bForceReload = false) override;

	/**
	 * Saves both the base UGameUserSettings and all sub-settings objects.
	 * Each sub-setting must explicitly call SaveConfig to persist its values.
	 */
	virtual void SaveSettings() override;
};
