/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameSettingValue.h"
#include "Input/SSInputLocalPlayerSubsystem.h"
#include "SSKeyInputSetting.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSKeyInputSetting : public UGameSettingValue
{
	GENERATED_BODY()
	
public:

	bool InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FKeyMappingRow& MappingData,
		FGameplayTag InGameModeControlTag, const FPlayerMappableKeyQueryOptions& InQueryOptions);

	FPlayerKeyMapping GetKeyFromSlot(const EPlayerMappableKeySlot InSlot) const;
	
	FText GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const;

	// Must : Pure virtual implementation
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;
	// ~ Must : Pure virtual implementation
	
	bool ChangeBinding(int32 InKeyBindSlot, FKey NewKey);
	void GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const;

	// Check if gamepad
	bool bGamepad = false;
	bool IsGamepad() const { return bGamepad; }
	
	const FKeyMappingRow* FindKeyMappingRow() const;
	UEnhancedPlayerMappableKeyProfile* FindMappableKeyProfile() const;
	UEnhancedInputUserSettings* GetUserSettings() const;
	
protected:

	virtual void OnInitialized() override;

	FGameplayTag GameModeControlTag;
	
	/** The name of this action's mappings */
	FName ActionMappingName;

	/** The query options to filter down keys on this setting for */
	FPlayerMappableKeyQueryOptions QueryOptions;

	/** The profile identifier that this key setting is from */
	FString ProfileIdentifierId;

	/** Store the initial key mappings that are set on this for each slot */
	TMap<EPlayerMappableKeySlot, FKey> InitialKeyMappings;
};
