/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSInputStaticsLibrary.h"

#include "CommonUITypes.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Texture2D.h"
#include "EnhancedInput/Public/PlayerMappableKeySettings.h"
#include "Input/SSInputLocalPlayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/StyleDefaults.h"

ULocalPlayer* USSInputStaticsLibrary::GetLocalPlayer(const UObject* WorldContextObject)
{
	if (auto* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		return PlayerController->GetLocalPlayer();
	}
	return nullptr;
}

const UCommonInputSubsystem* USSInputStaticsLibrary::GetCommonInputSubsystem(const UObject* WorldContextObject)
{
	return UCommonInputSubsystem::Get(GetLocalPlayer(WorldContextObject));
}

USSInputLocalPlayerSubsystem* USSInputStaticsLibrary::GetInputLocalPlayerSubsystem(const UObject* WorldContextObject)
{
	return GetLocalPlayer(WorldContextObject)->GetSubsystem<USSInputLocalPlayerSubsystem>();
}

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

	if (FirstKeyForCurrentInput.IsValid())
	{
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

	if (FirstKeyForCurrentInput.IsValid())
	{
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

	FName MappedKeyName;
	
	auto& PlayerMappableKeySettings = InputAction->GetPlayerMappableKeySettings();
	if (PlayerMappableKeySettings)
	{
		MappedKeyName = PlayerMappableKeySettings->Name;
	}
	
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
	if (FirstKeyForCurrentInput.IsValid() && UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, FirstKeyForCurrentInput, CommonInputSubsystem->GetCurrentInputType(),
	                                                                                               CommonInputSubsystem->GetCurrentGamepadName()))
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
	if (FirstKeyForCurrentInput.IsValid() && UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, FirstKeyForCurrentInput, InputType,
	                                                                                               CommonInputSubsystem->GetCurrentGamepadName()))
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

UTexture2D* USSInputStaticsLibrary::GetGamepadInputIconFromKey(const UObject* WorldContextObject, const FKey& Key, const FName& GamepadName)
{
	FSlateBrush SlateBrush;
	if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, Key, ECommonInputType::Gamepad, GamepadName))
	{
		if (UTexture2D* Texture = Cast<UTexture2D>(SlateBrush.GetResourceObject()))
		{
			return Texture;
		}
	}
	return nullptr;
}

UTexture2D* USSInputStaticsLibrary::GetInputIconFromKey(const UObject* WorldContextObject, const FKey& Key)
{
	if (const UCommonInputSubsystem* CommonInputSubsystem = GetCommonInputSubsystem(WorldContextObject))
	{
		return GetInputIconFromInputTypeKey(WorldContextObject, CommonInputSubsystem->GetCurrentInputType(),Key);
	}
	return nullptr;
}

UTexture2D* USSInputStaticsLibrary::GetInputIconFromInputTypeKey(const UObject* WorldContextObject, ECommonInputType InputType, const FKey& Key)
{
	if (const UCommonInputSubsystem* CommonInputSubsystem = GetCommonInputSubsystem(WorldContextObject))
	{
		FSlateBrush SlateBrush;
		if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, Key, InputType, CommonInputSubsystem->GetCurrentGamepadName()))
		{
			if (UTexture2D* Texture = Cast<UTexture2D>(SlateBrush.GetResourceObject()))
			{
				return Texture;
			}
		}
	}
	return nullptr;
}

void USSInputStaticsLibrary::SetGameSuspendInput(const UObject* WorldContextObject, FGameplayTag Tag, bool bSuspend)
{
	if (auto* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		auto* SSInputLocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<USSInputLocalPlayerSubsystem>();

		if (bSuspend)
		{
			SSInputLocalPlayerSubsystem->SuspendInputForTag(Tag);
		}
		else
		{
			SSInputLocalPlayerSubsystem->ResumeInputForTag(Tag);
		}
	}
}

bool USSInputStaticsLibrary::InterpretRightAnalog(const FAnalogInputEvent& AnalogInputEvent, bool InRight, float Sensibility, float& OutValue)
{
	// Determine which axis to read: RightX if true, RightY if false
	const FKey KeyToCapture = (InRight) ? EKeys::Gamepad_RightX : EKeys::Gamepad_RightY;

	// Check if the input event matches the expected key
	if (AnalogInputEvent.GetKey() == KeyToCapture)
	{
		// Scale the input value by sensitivity and return it
		OutValue = AnalogInputEvent.GetAnalogValue() * Sensibility;
		return true;
	}

	// Input key doesn't match; ignore
	return false;
}

bool USSInputStaticsLibrary::InterpretRightAnalogDZ(const FAnalogInputEvent& AnalogInputEvent, bool InRight, float Sensibility, float DeadZone, float& OutValue)
{
	const FKey KeyToCapture = (InRight) ? EKeys::Gamepad_RightX : EKeys::Gamepad_RightY;
	if (AnalogInputEvent.GetKey() == KeyToCapture)
	{
		const float Value = AnalogInputEvent.GetAnalogValue();

		// Ignore values within the deadzone threshold
		if (FMath::Abs(Value) < DeadZone) return false;

		OutValue = Value * Sensibility;
		return true;
	}
	return false;
}

bool USSInputStaticsLibrary::InterpretLeftAnalogMove(const FAnalogInputEvent& AnalogInputEvent, float Sensibility, FVector2D& OutValue)
{
	// Delegates to the DZ version with a deadzone of 0.0
	return InterpretLeftAnalogMoveDZ(AnalogInputEvent, Sensibility, 0.0f, OutValue);
}

bool USSInputStaticsLibrary::InterpretLeftAnalogMoveDZ(const FAnalogInputEvent& AnalogInputEvent, float Sensibility, float DeadZone, FVector2D& OutValue)
{
	// Initialize output to zeroed vector
	OutValue = FVector2D();

	// Check if the key corresponds to either X or Y axis of the left stick
	if (AnalogInputEvent.GetKey() == EKeys::Gamepad_LeftX || AnalogInputEvent.GetKey() == EKeys::Gamepad_LeftY)
	{
		const float AnalogValue = AnalogInputEvent.GetAnalogValue();

		// Ignore input if under deadzone threshold
		if (FMath::Abs(AnalogValue) < DeadZone)
			return false;

		const float Value = AnalogValue * Sensibility;

		// Assign to correct axis in output vector
		if (AnalogInputEvent.GetKey() == EKeys::Gamepad_LeftX)
		{
			OutValue.X = Value;
			return true;
		}

		// Invert Y axis to match common forward/up conventions
		OutValue.Y = -Value;
		return true;
	}

	return InterpretAnalogMoveDZ(AnalogInputEvent, false, Sensibility, DeadZone, OutValue);
}

bool USSInputStaticsLibrary::InterpretRightAnalogMove(const FAnalogInputEvent& AnalogInputEvent, float Sensibility, FVector2D& OutValue)
{
	return InterpretAnalogMoveDZ(AnalogInputEvent, true, Sensibility, 0.01f, OutValue);
}

bool USSInputStaticsLibrary::InterpretAnalogMoveDZ(const FAnalogInputEvent& AnalogInputEvent, bool bRight, float Sensibility, float DeadZone, FVector2D& OutValue)
{
	// Initialize output to zeroed vector
	OutValue = FVector2D();

	// Check if the key corresponds to either X or Y axis of the left stick
	FKey LeftKey = (bRight) ?  EKeys::Gamepad_RightX : EKeys::Gamepad_LeftX;
	bool bValidKey = false;
	if (bRight)
	{
		bValidKey = (AnalogInputEvent.GetKey() == EKeys::Gamepad_RightX || AnalogInputEvent.GetKey() == EKeys::Gamepad_RightY);
	}
	else
	{
		bValidKey = (AnalogInputEvent.GetKey() == EKeys::Gamepad_LeftX || AnalogInputEvent.GetKey() == EKeys::Gamepad_LeftY);
	}
	
	if (bValidKey)
	{
		const float AnalogValue = AnalogInputEvent.GetAnalogValue();

		// Ignore input if under deadzone threshold
		if (FMath::Abs(AnalogValue) < DeadZone)
			return false;

		const float Value = AnalogValue * Sensibility;

		// Assign to correct axis in output vector
		if (AnalogInputEvent.GetKey() == LeftKey)
		{
			OutValue.X = Value;
			return true;
		}

		// Invert Y axis to match common forward/up conventions
		OutValue.Y = -Value;
		return true;
	}

	return false;
}
