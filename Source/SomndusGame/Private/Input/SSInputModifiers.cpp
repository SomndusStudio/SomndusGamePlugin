/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSInputModifiers.h"

#include "EnhancedPlayerInput.h"
#include "Player/SSLocalPlayer.h"
#include "Settings/SSCommonGameUserSettings.h"
#include "Settings/SSGamepadUserSettings.h"
#include "Settings/SSGameUserSettings.h"

USSAimSensitivityData::USSAimSensitivityData(const FObjectInitializer& ObjectInitializer)
{
	SensitivityMap =
	{
		{ ESSGamepadSensitivity::Slow,			0.5f },
		{ ESSGamepadSensitivity::SlowPlus,		0.75f },
		{ ESSGamepadSensitivity::SlowPlusPlus,	0.9f },
		{ ESSGamepadSensitivity::Normal,		1.0f },
		{ ESSGamepadSensitivity::NormalPlus,	1.1f },
		{ ESSGamepadSensitivity::NormalPlusPlus,1.25f },
		{ ESSGamepadSensitivity::Fast,			1.5f },
		{ ESSGamepadSensitivity::FastPlus,		1.75f },
		{ ESSGamepadSensitivity::FastPlusPlus,	2.0f },
		{ ESSGamepadSensitivity::Insane,		2.5f },
	};
}

const float USSAimSensitivityData::SensitivityEnumToFloat(const ESSGamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}


//////////////////////////////////////////////////////////////////////
// SSInputModifiersHelpers

namespace SSInputModifiersHelpers
{
	/** Returns the owning LyraLocalPlayer of an Enhanced Player Input pointer */
	static USSLocalPlayer* GetLocalPlayer(const UEnhancedPlayerInput* PlayerInput)
	{
		if (PlayerInput)
		{
			if (const APlayerController* PC = Cast<APlayerController>(PlayerInput->GetOuter()))
			{
				return Cast<USSLocalPlayer>(PC->GetLocalPlayer());
			}
		}
		return nullptr;
	}
	
}

FInputActionValue USSInputModifierDeadZone::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
	FInputActionValue CurrentValue, float DeltaTime)
{
	const EInputActionValueType ValueType = CurrentValue.GetValueType();
	const USSLocalPlayer* LocalPlayer = SSInputModifiersHelpers::GetLocalPlayer(PlayerInput);
	
	if (ValueType == EInputActionValueType::Boolean || !LocalPlayer)
	{
		return CurrentValue;
	}

	if (!GamepadSettings)
	{
		auto* GameUserSettings = USSCommonGameUserSettings::Get();
		ensure(GameUserSettings);
		GamepadSettings = GameUserSettings->GetGamepadSettings();
	}
	
	float LowerThreshold =
		(DeadzoneStick == ESSDeadzoneStick::MoveStick) ? 
		GamepadSettings->GetGamepadMoveStickDeadZone() :
		GamepadSettings->GetGamepadLookStickDeadZone();
	
	LowerThreshold = FMath::Clamp(LowerThreshold, 0.0f, 1.0f);
	
	auto DeadZoneLambda = [LowerThreshold, this](const float AxisVal)
	{
		// We need to translate and scale the input to the +/- 1 range after removing the dead zone.
		return FMath::Min(1.f, (FMath::Max(0.f, FMath::Abs(AxisVal) - LowerThreshold) / (UpperThreshold - LowerThreshold))) * FMath::Sign(AxisVal);
	};

	FVector NewValue = CurrentValue.Get<FVector>();
	switch (Type)
	{
	case EDeadZoneType::Axial:
		NewValue.X = DeadZoneLambda(NewValue.X);
		NewValue.Y = DeadZoneLambda(NewValue.Y);
		NewValue.Z = DeadZoneLambda(NewValue.Z);
		break;
	case EDeadZoneType::Radial:
		if (ValueType == EInputActionValueType::Axis3D)
		{
			NewValue = NewValue.GetSafeNormal() * DeadZoneLambda(NewValue.Size());
		}
		else if (ValueType == EInputActionValueType::Axis2D)
		{
			NewValue = NewValue.GetSafeNormal2D() * DeadZoneLambda(NewValue.Size2D());
		}
		else
		{
			NewValue.X = DeadZoneLambda(NewValue.X);
		}
		break;
	}

	return NewValue;
}

FLinearColor USSInputModifierDeadZone::GetVisualizationColor_Implementation(FInputActionValue SampleValue,
	FInputActionValue FinalValue) const
{
	// Taken from UInputModifierDeadZone::GetVisualizationColor_Implementation
	if (FinalValue.GetValueType() == EInputActionValueType::Boolean || FinalValue.GetValueType() == EInputActionValueType::Axis1D)
	{
		return FLinearColor(FinalValue.Get<float>() == 0.f ? 1.f : 0.f, 0.f, 0.f);
	}
	return FLinearColor((FinalValue.Get<FVector2D>().X == 0.f ? 0.5f : 0.f) + (FinalValue.Get<FVector2D>().Y == 0.f ? 0.5f : 0.f), 0.f, 0.f);
}

////////////////////////////////////////////////////////////////////
// USSInputModifierGamepadSensitivity
FInputActionValue USSInputModifierGamepadSensitivity::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                               FInputActionValue CurrentValue, float DeltaTime)
{
	// You can't scale a boolean action type
	const USSLocalPlayer* LocalPlayer = SSInputModifiersHelpers::GetLocalPlayer(PlayerInput);
	
	if (CurrentValue.GetValueType() == EInputActionValueType::Boolean || !LocalPlayer || !SensitivityLevelTable)
	{
		return CurrentValue;
	}

	if (!GamepadSettings)
	{
		auto* GameUserSettings = USSCommonGameUserSettings::Get();
		ensure(GameUserSettings);
		GamepadSettings = GameUserSettings->GetGamepadSettings();
	}
	const ESSGamepadSensitivity Sensitivity = GamepadSettings->GetLookSensitivityPreset();

	const float Scalar = SensitivityLevelTable->SensitivityEnumToFloat(Sensitivity);

	return CurrentValue.Get<FVector>() * Scalar;
}

////////////////////////////////////////////////////////////////////
// USSInputModifierMouseSensitivity
FInputActionValue USSInputModifierMouseSensitivity::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	// You can't scale a boolean action type
	const USSLocalPlayer* LocalPlayer = SSInputModifiersHelpers::GetLocalPlayer(PlayerInput);
	
	if (CurrentValue.GetValueType() == EInputActionValueType::Boolean || !LocalPlayer)
	{
		return CurrentValue;
	}

	if (!MouseKeyboardUserSettings)
	{
		auto* GameUserSettings = USSCommonGameUserSettings::Get();
		ensure(GameUserSettings);
		MouseKeyboardUserSettings = GameUserSettings->GetMouseKeyboardUserSettings();
	}

	FVector NewValue = CurrentValue.Get<FVector>();

	NewValue.X *= MouseKeyboardUserSettings->GetMouseSensitivityX();
	NewValue.Y *= MouseKeyboardUserSettings->GetMouseSensitivityY();

	return NewValue;
}

////////////////////////////////////////////////////////////////////
// USSInputModifierInversion
FInputActionValue USSInputModifierInversion::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                      FInputActionValue CurrentValue, float DeltaTime)
{
	const USSLocalPlayer* LocalPlayer = SSInputModifiersHelpers::GetLocalPlayer(PlayerInput);
	if (!LocalPlayer)
	{
		return CurrentValue;
	}

	FVector NewValue = CurrentValue.Get<FVector>();

	// Gamepad
	if (bIsGamepad)
	{
		if (!GamepadSettings)
		{
			auto* GameUserSettings = USSCommonGameUserSettings::Get();
			ensure(GameUserSettings);
			GamepadSettings = GameUserSettings->GetGamepadSettings();
		}
	
		if (GamepadSettings->GetInvertVerticalAxis())
		{
			NewValue.Y *= -1.0f;
		}
	
		if (GamepadSettings->GetInvertHorizontalAxis())
		{
			NewValue.X *= -1.0f;
		}
	}
	// keyboard
	else
	{
		if (!MouseKeyboardUserSettings)
		{
			auto* GameUserSettings = USSCommonGameUserSettings::Get();
			ensure(GameUserSettings);
			MouseKeyboardUserSettings = GameUserSettings->GetMouseKeyboardUserSettings();
		}
		if (MouseKeyboardUserSettings->GetInvertMouse())
		{
			NewValue.Y *= -1.0f;
		}
		if (MouseKeyboardUserSettings->GetInvertMouseX())
		{
			NewValue.X *= -1.0f;
		}
	}
	
	return NewValue * FVector(1.f, bY ? -1.f : 1.f, 1.f);
}
