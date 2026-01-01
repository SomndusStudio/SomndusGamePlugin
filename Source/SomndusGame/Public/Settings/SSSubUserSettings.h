/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SSSubUserSettings.generated.h"

/**
 * @class USSSubUserSettings
 * Base class for user-specific settings in the game. Provides functionality for managing
 *        settings, resetting them, and handling change detection and dirty state.
 *
 * This class acts as the foundation for more specific subclasses that define individual categories
 * of user settings. It includes support for setting defaults, loading, saving, and detecting changes
 * in values to facilitate configuration management.
 */
UCLASS()
class SOMNDUSGAME_API USSSubUserSettings : public UObject
{
	GENERATED_BODY()

public:

	virtual void SetToDefaults();
	virtual void LoadSettings();
	virtual void SaveSettings();
	
	////////////////////////////////////////////////////////
	/// Dirty and Change Reporting
protected:
	template<typename T>
	bool ChangeValueAndDirty(T& CurrentValue, const T& NewValue)
	{
		if (CurrentValue != NewValue)
		{
			CurrentValue = NewValue;
			bIsDirty = true;
			
			//OnSettingChanged.Broadcast(this);
			
			return true;
		}

		return false;
	}

	bool bIsDirty = false;
};
