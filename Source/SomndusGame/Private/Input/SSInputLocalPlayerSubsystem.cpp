/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSInputLocalPlayerSubsystem.h"

#include "CommonInputSubsystem.h"
#include "CommonUIExtensions.h"
#include "CommonUITypes.h"
#include "EnhancedInputSubsystems.h"
#include "SSLog.h"
#include "Engine/LocalPlayer.h"
#include "Input/SSEnhancedPlayerInput.h"
#include "Settings/SSCommonGameUserSettings.h"

USSInputLocalPlayerSubsystem::USSInputLocalPlayerSubsystem()
{
	GamepadTypeNames = {
		"XSX",
		"PS4",
		"PS5",
		"SwitchPro"
	};

	bOverrideGamepadUsed = false;
}

void USSInputLocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load global mapping context
	GenericUIMappingContext = GenericUIMappingContextSoft.LoadSynchronous();

	// Listen gamepad user settings change
	auto* GameUserSettings = USSCommonGameUserSettings::Get();
	if (GameUserSettings)
	{
		GameUserSettings->GetGamepadSettings()->OnGamepadInputNeedsUpdate.AddUniqueDynamic(this, &ThisClass::ApplyOverrideGamepadName);
	}
	else
	{
		UE_LOG(LogSomndusGame, Error, TEXT("Invalid GameUserSetting (should inherit of USSCommonGameUserSettings)"));
	}

	AfterSettingsLoaded();
}

void USSInputLocalPlayerSubsystem::AfterSettingsLoaded()
{
	ApplyOverrideGamepadName();
}

void USSInputLocalPlayerSubsystem::UpdateKeyMappingCache(const UEnhancedInputLocalPlayerSubsystem* EISubsystem)
{
	if (const auto* PlayerInput = Cast<USSEnhancedPlayerInput>(EISubsystem->GetPlayerInput()))
	{
		for (const FEnhancedActionKeyMapping& Mapping : PlayerInput->GetPublicEnhancedActionMappings())
		{
			KeyMappingRows.Add(Mapping);
		}
	}
	/*
	const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
	for (const auto& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
	{
		for (const auto& MappingRowPair : ProfilePair.Value->GetPlayerMappingRows())
		{
			KeyMappingRows.Add(MappingRowPair.Key, MappingRowPair.Value);
		}
	}
	*/
}

void USSInputLocalPlayerSubsystem::UpdateKeyMappingCache()
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	UpdateKeyMappingCache(EISubsystem);
}

bool USSInputLocalPlayerSubsystem::FirstTimeMappingContext(const UInputMappingContext* MappingContext)
{
	for (const auto& PullToken: InputMappingContextPullTokens)
	{
		if (PullToken.MappingContext == MappingContext) return false;
	}
	return true;
}

bool USSInputLocalPlayerSubsystem::ContainsMappingContextPull(const UInputMappingContext* MappingContext)
{
	for (const auto& PullToken: InputMappingContextPullTokens)
	{
		if (PullToken.MappingContext == MappingContext) return true;
	}

	return false;
}

bool USSInputLocalPlayerSubsystem::PullContainsMappingContext(const UInputMappingContext* MappingContext, FString Token)
{
	for (const auto& PullToken: InputMappingContextPullTokens)
	{
		if (PullToken.MappingContext == MappingContext && PullToken.Token == Token) return true;
	}
	return false;
}

void USSInputLocalPlayerSubsystem::AddPullMappingContext(const UInputMappingContext* MappingContext, FString Token)
{
	FSSInputMappingContextPullToken PullToken;
	PullToken.MappingContext = MappingContext;
	PullToken.Token = Token;
	InputMappingContextPullTokens.Add(PullToken);	
}

void USSInputLocalPlayerSubsystem::PullMappingContext(const UInputMappingContext* MappingContext, int32 InputMappingPriority, FString Token)
{
	// If first time same input mapping context
	if (FirstTimeMappingContext(MappingContext))
	{
		if (CommonUI::IsEnhancedInputSupportEnabled())
		{
			if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					InputSystem->AddMappingContext(MappingContext, InputMappingPriority);
				}
			}
		}

		AddPullMappingContext(MappingContext, Token);
		return;
	}

	// Avoid duplicate entry
	if (!PullContainsMappingContext(MappingContext, Token))
	{
		AddPullMappingContext(MappingContext, Token);
	}

}

void USSInputLocalPlayerSubsystem::PullGlobalMappingContext(FString Token)
{
	PullMappingContext(GenericUIMappingContext, 0, Token);
}

void USSInputLocalPlayerSubsystem::PopGlobalMappingContext(FString Token)
{
	PopMappingContext(GenericUIMappingContext, Token);
}

void USSInputLocalPlayerSubsystem::PopMappingContext(const UInputMappingContext* MappingContext, FString Token)
{
	// Remove from pull array
	InputMappingContextPullTokens.RemoveAll([MappingContext, Token](const FSSInputMappingContextPullToken& Value)
	{
		return Value.MappingContext == MappingContext && Value.Token == Token;
	});
	
	// If pop all same mapping context
	if (!ContainsMappingContextPull(MappingContext))
	{
		if (CommonUI::IsEnhancedInputSupportEnabled())
		{
			if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					InputSystem->RemoveMappingContext(MappingContext);
				}
			}
		}
	}

}

void USSInputLocalPlayerSubsystem::SuspendInputForUIAnimation()
{
	// if already suspended do nothing
	if (!CommonUIAnimationInputSuspendToken.IsNone())
	{
		return;
	}
	CommonUIAnimationInputSuspendToken = UCommonUIExtensions::SuspendInputForPlayer(GetLocalPlayer()->GetPlayerController(GetWorld()), "CommonUIAnimation");

	UE_LOG(LogSomndusGame, Log, TEXT("SSInput: SuspendInputForUIAnimation"));
}

void USSInputLocalPlayerSubsystem::ResumeInputForUIAnimation()
{
	if (CommonUIAnimationInputSuspendToken.IsNone())
	{
		return;
	}
	UCommonUIExtensions::ResumeInputForPlayer(GetLocalPlayer()->GetPlayerController(GetWorld()), CommonUIAnimationInputSuspendToken);
	CommonUIAnimationInputSuspendToken = FName();

	UE_LOG(LogSomndusGame, Log, TEXT("SSInput: ResumeInputForUIAnimation"));
}

void USSInputLocalPlayerSubsystem::SuspendInputForGameplay()
{
	// if already suspended do nothing
	if (!CommonGameplayInputSuspendToken.IsNone())
	{
		return;
	}
	CommonGameplayInputSuspendToken = UCommonUIExtensions::SuspendInputForPlayer(GetLocalPlayer()->GetPlayerController(GetWorld()), "Gameplay");

	UE_LOG(LogSomndusGame, Log, TEXT("SSInput: SuspendInputForGameplay"));
}

void USSInputLocalPlayerSubsystem::ResumeInputForGameplay()
{
	if (CommonGameplayInputSuspendToken.IsNone())
	{
		return;
	}
	UCommonUIExtensions::ResumeInputForPlayer(GetLocalPlayer()->GetPlayerController(GetWorld()), CommonGameplayInputSuspendToken);
	CommonGameplayInputSuspendToken = FName();
	
	UE_LOG(LogSomndusGame, Log, TEXT("SSInput: ResumeInputForGameplay"));
}

void USSInputLocalPlayerSubsystem::SuspendInputForTag(FGameplayTag Tag)
{
	// if already suspended do nothing
	if (GameSuspendTokens.Contains(Tag))
	{
		return;
	}
	FName SuspendToken = UCommonUIExtensions::SuspendInputForPlayer(GetLocalPlayer()->GetPlayerController(GetWorld()), Tag.GetTagName());
	GameSuspendTokens.Add(Tag, SuspendToken);
	
	UE_LOG(LogSomndusGame, Log, TEXT("SSInput: SuspendInputForTag -> %s"), *Tag.GetTagName().ToString());
}

void USSInputLocalPlayerSubsystem::ResumeInputForTag(FGameplayTag Tag)
{
	if (!GameSuspendTokens.Contains(Tag))
	{
		return;
	}
	FName SuspendToken = GameSuspendTokens.FindChecked(Tag);
	UCommonUIExtensions::ResumeInputForPlayer(GetLocalPlayer()->GetPlayerController(GetWorld()), SuspendToken);
	GameSuspendTokens.Remove(Tag);

	UE_LOG(LogSomndusGame, Log, TEXT("SSInput: ResumeInputForTag -> %s"), *Tag.GetTagName().ToString());
}

void USSInputLocalPlayerSubsystem::ApplyOverrideGamepadName()
{
#if SUPPORT_OVERRIDE_GAMEPAD_TYPE
	if (auto* CommonInputSubsystem = UCommonInputSubsystem::Get(GetLocalPlayer()))
	{
		auto* GameUserSettings = USSCommonGameUserSettings::Get();
		if (GameUserSettings->GetGamepadSettings()->GetOverrideGamepadInputType())
		{
			FName GamepadName = GamepadTypeNames[GameUserSettings->GetGamepadSettings()->GetGamepadInputTypeIndex()];

			UE_LOG(LogSomndusInput, Log, TEXT("Enable overriden gamepad input type to %s"), *GamepadName.ToString());
			
			CommonInputSubsystem->SetGamepadInputTypeOverridden(true, GamepadName);

			bOverrideGamepadUsed = true;
		}
		else
		{
			if (!bOverrideGamepadUsed)
			{
				UE_LOG(LogSomndusInput, Log, TEXT("Overriden gamepad not used yet, no fallback needed"));
				return;
			}
			// restore
			UE_LOG(LogSomndusInput, Log, TEXT("Disable overriden gamepad input type"));
			
			CommonInputSubsystem->SetGamepadInputTypeOverridden(false, "");
			
			bOverrideGamepadUsed = false;
		}
	}
#endif
}
