// ©2025 Schartier Isaac. All rights reserved.


#include "Settings/Keyboard/SSKeyInputSetting.h"


#include "EnhancedInputSubsystems.h"
#include "PlayerMappableKeySettings.h"
#include "Engine/LocalPlayer.h"
#include "Input/SSInputStaticsLibrary.h"

#define LOCTEXT_NAMESPACE "SomndusGame"

bool USSKeyInputSetting::InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile,
	const FKeyMappingRow& MappingData, FGameplayTag InGameModeControlTag, const FPlayerMappableKeyQueryOptions& InQueryOptions)
{
	check(KeyProfile);

	GameModeControlTag = InGameModeControlTag;
	ProfileIdentifier = KeyProfile->GetProfileIdentifer();
	QueryOptions = InQueryOptions;

	// Store to know i it's gamepad
	bGamepad = QueryOptions.KeyToMatch.IsGamepadKey();
	
	// To know if should be used for setting (if yound on valid key (for filtering gamepad/keyboard)
	bool bHaveOneKeyValid = false;
	
	for (const FPlayerKeyMapping& Mapping : MappingData.Mappings)
	{
		// Only add mappings that pass the query filters that have been provided upon creation
		if (!KeyProfile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
		{
			continue;
		}
		
		ActionMappingName = Mapping.GetMappingName();
		InitialKeyMappings.Add(Mapping.GetSlot(), Mapping.GetCurrentKey());
		const FText& MappingDisplayName =Mapping.GetDisplayName();
		
		if (!MappingDisplayName.IsEmpty())
		{
			SetDisplayName(MappingDisplayName);
			bHaveOneKeyValid = true;
		}
	}

	// If found no one valid key
	if (!bHaveOneKeyValid)
	{
		return false;
	}
	
	const FString PlatformStr = IsGamepad() ? "Gamepad_" : "Keyboard_";

	const FString NameString = FString::Printf(TEXT("%sInput_%s_%s"), *PlatformStr,
		*ActionMappingName.ToString(), *GameModeControlTag.GetTagName().ToString());
	
	SetDevName(*NameString);

	return true;
}

FPlayerKeyMapping USSKeyInputSetting::GetKeyFromSlot(const EPlayerMappableKeySlot InSlot) const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		FPlayerMappableKeyQueryOptions QueryOptionsForSlot = QueryOptions;
		// For now ignore slot as filter from keyboard/Gamepad
		//QueryOptionsForSlot.SlotToMatch = InSlot;
		
		if (const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptionsForSlot))
				{
					return Mapping;
				}
			}
		}
	}
	
	return FPlayerKeyMapping();
}

FText USSKeyInputSetting::GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const
{
	const FPlayerKeyMapping FoundPlayerKeyMapping = GetKeyFromSlot(InSlot);
	if (FoundPlayerKeyMapping.IsValid())
	{
		return FoundPlayerKeyMapping.GetCurrentKey().GetDisplayName();
	}
	
	return EKeys::Invalid.GetDisplayName();
}

void USSKeyInputSetting::StoreInitial()
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		if(const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
				{
					ActionMappingName = Mapping.GetMappingName();
					InitialKeyMappings.Add(Mapping.GetSlot(), Mapping.GetCurrentKey());	
				}
			}
		}
	}
}

void USSKeyInputSetting::ResetToDefault()
{
	if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;

		FGameplayTagContainer FailureReason;
		Settings->ResetAllPlayerKeysInRow(Args, FailureReason);

		NotifySettingChanged(EGameSettingChangeReason::Change);
	}
}

void USSKeyInputSetting::RestoreToInitial()
{
	for (TPair<EPlayerMappableKeySlot, FKey> Pair : InitialKeyMappings)
	{
		ChangeBinding((int32)Pair.Key, Pair.Value);	
	}
}

bool USSKeyInputSetting::ChangeBinding(int32 InKeyBindSlot, FKey NewKey)
{
	EPlayerMappableKeySlot KeySlot = (EPlayerMappableKeySlot) (static_cast<uint8>(InKeyBindSlot));
	FPlayerKeyMapping OriginalKeyMapping = GetKeyFromSlot(KeySlot);
	FKey OriginalKey = OriginalKeyMapping.GetCurrentKey();
	
	// Setup arg
	FMapPlayerKeyArgs Args = {};
	Args.MappingName = ActionMappingName;
	Args.Slot = KeySlot;
	Args.NewKey = NewKey;
	// If you want to, you can additionally specify this mapping to only be applied to a certain hardware device or key profile
	//Args.ProfileId =
	//Args.HardwareDeviceId =
		
	if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{	
		// PS : I Changed the order to apply input to avoid loosing default ordering especially in UI

		// Store bounds before apply current key first
		TArray<FName> BoundActionNames;
		GetAllMappedActionsFromKey(InKeyBindSlot, NewKey, BoundActionNames);
		
		// New original key
		FGameplayTagContainer FailureReason;
		Settings->MapPlayerKey(Args, FailureReason);

		// Replace already bound key
		for (const auto& BoundActionName : BoundActionNames)
		{
			FGameplayTagContainer BoundFailureReason;
			FMapPlayerKeyArgs BoundKeyArgs = {};
			BoundKeyArgs.MappingName = BoundActionName;
			BoundKeyArgs.Slot = KeySlot;
			BoundKeyArgs.NewKey = OriginalKey;
			Settings->MapPlayerKey(BoundKeyArgs, BoundFailureReason);
		}

		// Try to sync an input binding like Weapon Ability -> Spirit Ability
		USSInputStaticsLibrary::SyncInputMapping(LocalPlayer, ActionMappingName, KeySlot, NewKey, Settings);
		
		NotifySettingChanged(EGameSettingChangeReason::Change);

		return true;
	}

	return false;
}

void USSKeyInputSetting::GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key,
	TArray<FName>& OutActionNames) const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		// Only in good Game Mode Control
		TArray<FName> MappingNames;
		Profile->GetMappingNamesForKey(Key, MappingNames);

		auto* InputLocalPlayer = USSInputStaticsLibrary::GetInputLocalPlayerSubsystem(this);
		for (const auto& MappingName : MappingNames)
		{
			if (InputLocalPlayer->MappingNameIsInGameModeControl(MappingName, GameModeControlTag))
			{
				OutActionNames.AddUnique(MappingName);
			}
		}
	}
}

const FKeyMappingRow* USSKeyInputSetting::FindKeyMappingRow() const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		return Profile->FindKeyMappingRow(ActionMappingName);
	}

	ensure(false);
	return nullptr;
}

UEnhancedPlayerMappableKeyProfile* USSKeyInputSetting::FindMappableKeyProfile() const
{
	if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{
		return Settings->GetKeyProfileWithIdentifier(ProfileIdentifier);
	}
	
	ensure(false);
	return nullptr;
}

UEnhancedInputUserSettings* USSKeyInputSetting::GetUserSettings() const
{
	// Map the key to the player key profile
	if (UEnhancedInputLocalPlayerSubsystem* System = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		return System->GetUserSettings();
	}

	return nullptr;
}

void USSKeyInputSetting::OnInitialized()
{
	DynamicDetails = FGetGameSettingsDetails::CreateLambda([this](ULocalPlayer&)
	{
		if (const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			if (Row->HasAnyMappings())
			{
				return FText::Format(LOCTEXT("DynamicDetails_KeyboardInputAction", "Bindings for {0}"), Row->Mappings.begin()->GetDisplayName());
			}
		}
		return FText::GetEmpty();
	});

	Super::OnInitialized();
}

#undef LOCTEXT_NAMESPACE