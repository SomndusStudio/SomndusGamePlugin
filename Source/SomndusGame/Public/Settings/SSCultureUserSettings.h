/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSSubUserSettings.h"
#include "SSCultureUserSettings.generated.h"

/**
 * A user settings class used to handle culture and language changes.
 * Manages pending culture updates, resetting to default culture,
 * and applying culture/language configurations.
 */
UCLASS()
class SOMNDUSGAME_API USSCultureUserSettings : public USSSubUserSettings
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////
	// Culture / language
private:
	/** The pending culture to apply */
	UPROPERTY(Transient)
	FString PendingCulture;
	
	/* If true, resets the culture to default. */
	bool bResetToDefaultCulture = false;

public:
	/** Gets the pending culture */
	const FString& GetPendingCulture() const;

	/** Sets the pending culture to apply */
	void SetPendingCulture(const FString& NewCulture);

	/** Clears the pending culture to apply */
	void ClearPendingCulture();
	
	void ApplyCultureSettings();

	void ResetToDefaultCulture();
	bool ShouldResetToDefaultCulture() const { return bResetToDefaultCulture; }
	void ResetCultureToCurrentSettings();
};
