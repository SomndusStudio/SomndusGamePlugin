/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSInputStaticsLibrary.h"

#include "CommonUITypes.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/PlayerMappableKeySettings.h"
#include "Input/SSInputLocalPlayerSubsystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"

FText USSInputStaticsLibrary::GetKeyNameForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem,
                                                               const UInputAction* InputAction)
{
	if (!InputAction)
	{
		return FText::FromString("");
	}
	FKey FirstKeyForCurrentInput = CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), CommonInputSubsystem->GetCurrentInputType(), InputAction);

	if (!FirstKeyForCurrentInput.IsValid())
	{
		auto* SSInputLocalPlayerSubsystem = CommonInputSubsystem->GetLocalPlayer()->GetSubsystem<USSInputLocalPlayerSubsystem>();
		FirstKeyForCurrentInput = GetFirstKeyForInputType(SSInputLocalPlayerSubsystem, CommonInputSubsystem->GetCurrentInputType(), InputAction);
	}
	
	if (FirstKeyForCurrentInput.IsValid()){
		return FirstKeyForCurrentInput.GetDisplayName();
	}

	return FText::FromString("");
}

FText USSInputStaticsLibrary::GetKeyNameForInputActionType(const UCommonInputSubsystem* CommonInputSubsystem,
	const UInputAction* InputAction, bool bGamepad)
{
	if (!InputAction)
	{
		return FText::FromString("");
	}
	
	ECommonInputType InputType = ECommonInputType::MouseAndKeyboard;

	if (bGamepad)
	{
		InputType = ECommonInputType::Gamepad;
	}
	
	FKey FirstKeyForCurrentInput = CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), InputType, InputAction);

	if (!FirstKeyForCurrentInput.IsValid())
	{
		auto* SSInputLocalPlayerSubsystem = CommonInputSubsystem->GetLocalPlayer()->GetSubsystem<USSInputLocalPlayerSubsystem>();
		FirstKeyForCurrentInput = GetFirstKeyForInputType(SSInputLocalPlayerSubsystem, CommonInputSubsystem->GetCurrentInputType(), InputAction);
	}
	
	if (FirstKeyForCurrentInput.IsValid()){
		return FirstKeyForCurrentInput.GetDisplayName();
	}

	return FText::FromString("");
}

bool USSInputStaticsLibrary::IsKeyInInputAction(const UCommonInputSubsystem* CommonInputSubsystem,
	const UInputAction* InputAction, const UInputAction* OtherInputAction)
{
	ECommonInputType InputType = CommonInputSubsystem->GetCurrentInputType();
	FKey KeyA = CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), InputType, InputAction);
	FKey KeyB = CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), InputType, OtherInputAction);

	return KeyA == KeyB;
}

FKey USSInputStaticsLibrary::GetFirstKeyForInputType(const UCommonInputSubsystem* CommonInputSubsystem,
	ECommonInputType InputType, const TArray<FDataTableRowHandle>& InputActions)
{
	FKey Key;
	for (const FDataTableRowHandle& InputAction : InputActions)
	{
		if (const FCommonInputActionDataBase* InputActionData = CommonUI::GetInputActionData(InputAction))
		{
			const FCommonInputTypeInfo& TypeInfo = InputActionData->GetCurrentInputTypeInfo(CommonInputSubsystem);
			Key = TypeInfo.GetKey();
		}
	}

	return Key;
}

FKey USSInputStaticsLibrary::GetFirstKeyForInputType(const USSInputLocalPlayerSubsystem* SSInputLocalPlayer, ECommonInputType InputType, const UInputAction* InputAction)
{
	FKey FoundKey;
	
	if (!InputAction)
	{
		return FoundKey;
	}
	
	FName MappedKeyName = InputAction->GetPlayerMappableKeySettings()->Name;
	for (const auto& Mapping : SSInputLocalPlayer->KeyMappingRows)
	{
		if (Mapping.GetMappingName() != MappedKeyName)
		{
			continue;
		}
		FKey Key = Mapping.Key;
		if (Key.IsGamepadKey() && InputType == ECommonInputType::Gamepad)
		{
			FoundKey = Key;
			break;
		}
		if (!Key.IsGamepadKey() && InputType == ECommonInputType::MouseAndKeyboard)
		{
			FoundKey = Key;
			break;
		}
	}
	return FoundKey;
}

FSlateBrush USSInputStaticsLibrary::GetIconForEnhancedInputAction(const UCommonInputSubsystem* CommonInputSubsystem,
                                                                  const UInputAction* InputAction)
{
	if (!InputAction)
	{
		return *FStyleDefaults::GetNoBrush();
	}
	
	FKey FirstKeyForCurrentInput = CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), CommonInputSubsystem->GetCurrentInputType(), InputAction);

	if (!FirstKeyForCurrentInput.IsValid())
	{
		auto* SSInputLocalPlayerSubsystem = CommonInputSubsystem->GetLocalPlayer()->GetSubsystem<USSInputLocalPlayerSubsystem>();
		FirstKeyForCurrentInput = GetFirstKeyForInputType(SSInputLocalPlayerSubsystem, CommonInputSubsystem->GetCurrentInputType(), InputAction);
	}
	
	FSlateBrush SlateBrush;
	if (FirstKeyForCurrentInput.IsValid() && UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, FirstKeyForCurrentInput, CommonInputSubsystem->GetCurrentInputType(), CommonInputSubsystem->GetCurrentGamepadName()))
	{
		return SlateBrush;
	}

	return *FStyleDefaults::GetNoBrush();
}

FSlateBrush USSInputStaticsLibrary::GetIconForInputActionType(const UCommonInputSubsystem* CommonInputSubsystem,
                                                              const UInputAction* InputAction, bool bGamepad)
{
	if (!InputAction)
	{
		return *FStyleDefaults::GetNoBrush();
	}
	
	ECommonInputType InputType = ECommonInputType::MouseAndKeyboard;

	if (bGamepad)
	{
		InputType = ECommonInputType::Gamepad;
	}
	
	FKey FirstKeyForCurrentInput = CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), InputType, InputAction);

	if (!FirstKeyForCurrentInput.IsValid())
	{
		auto* SSInputLocalPlayerSubsystem = CommonInputSubsystem->GetLocalPlayer()->GetSubsystem<USSInputLocalPlayerSubsystem>();
		FirstKeyForCurrentInput = GetFirstKeyForInputType(SSInputLocalPlayerSubsystem, InputType, InputAction);
	}
	
	// KeyMappingRows
	FSlateBrush SlateBrush;
	if (FirstKeyForCurrentInput.IsValid() && UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, FirstKeyForCurrentInput, InputType, CommonInputSubsystem->GetCurrentGamepadName()))
	{
		return SlateBrush;
	}
	
	return *FStyleDefaults::GetNoBrush();
}

FSlateBrush USSInputStaticsLibrary::GetIconForKey(const UCommonInputSubsystem* CommonInputSubsystem, const FKey Key)
{
	if (!Key.IsValid())
	{
		return *FStyleDefaults::GetNoBrush();
	}
	
	ECommonInputType InputType = ECommonInputType::MouseAndKeyboard;
	if (Key.IsGamepadKey())
	{
		InputType = ECommonInputType::Gamepad;
	}
	
	FSlateBrush SlateBrush;
	if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, Key, InputType, CommonInputSubsystem->GetCurrentGamepadName()))
	{
		return SlateBrush;
	}

	return *FStyleDefaults::GetNoBrush();
}
