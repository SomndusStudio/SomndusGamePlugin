// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "SSLocalPlayer.generated.h"

class UGameUserSettings;

/**
 * Represents a local player with specific access to user settings and configurations
 * tailored for the USSLocalPlayer class. Inherits from UCommonLocalPlayer.
 */
UCLASS()
class SOMNDUSGAME_API USSLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public :
	/** Gets the local settings for this player, this is read from config files at process startup and is always valid */
	UFUNCTION()
	UGameUserSettings* GetGameUserSettings() const;

	template<typename T>
	T* GetTypedGameUserSettings() const
	{
		return Cast<T>(GetGameUserSettings());
	}
};
