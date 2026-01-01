/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "Engine/DataAsset.h"
#include "Settings/SSGamepadUserSettings.h"
#include "SSInputModifiers.generated.h"


class USSGamepadUserSettings;
/** Represents which stick that this deadzone is for, either the move or the look stick */
UENUM()
enum class ESSDeadzoneStick : uint8
{
	/** Deadzone for the movement stick */
	MoveStick = 0,

	/** Deadzone for the looking stick */
	LookStick = 1,
};

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSAimSensitivityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	USSAimSensitivityData(const FObjectInitializer& ObjectInitializer);
	
	const float SensitivityEnumToFloat(const ESSGamepadSensitivity InSensitivity) const;
	
protected:
	/** Map of SensitivityMap settings to their corresponding float */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESSGamepadSensitivity, float> SensitivityMap;
};

/**
 * This is a deadzone input modifier that will have it's thresholds driven by what is in the Lyra Shared game settings. 
 */
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Settings Driven Dead Zone"))
class USSInputModifierDeadZone : public UInputModifier
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Settings, Config)
	EDeadZoneType Type = EDeadZoneType::Radial;
	
	// Threshold above which input is clamped to 1
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Settings, Config)
	float UpperThreshold = 1.0f;

	/** Which stick this deadzone is for. This controls which setting will be used when calculating the deadzone */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Settings, Config)
	ESSDeadzoneStick DeadzoneStick = ESSDeadzoneStick::MoveStick;

protected:
	UPROPERTY()
	const USSGamepadUserSettings* GamepadSettings;
	
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	// Visualize as black when unmodified. Red when blocked (with differing intensities to indicate axes)
	// Mirrors visualization in https://www.gamasutra.com/blogs/JoshSutphin/20130416/190541/Doing_Thumbstick_Dead_Zones_Right.php.
	virtual FLinearColor GetVisualizationColor_Implementation(FInputActionValue SampleValue, FInputActionValue FinalValue) const override;
};

/**
 *  Applies a scalar modifier based on the current gamepad settings in  game settings.
 */
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Gamepad Sensitivity"))
class USSInputModifierGamepadSensitivity : public UInputModifier
{
	GENERATED_BODY()
public:
	
	/** Asset that gives us access to the float scalar value being used for sensitivty */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TObjectPtr<const USSAimSensitivityData> SensitivityLevelTable;

protected:
	UPROPERTY()
	const USSGamepadUserSettings* GamepadSettings;
	
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};

/**
 * Apply mouse/gamepad inversion from setting
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Aim Inversion"))
class SOMNDUSGAME_API USSInputModifierInversion : public UInputModifier
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Settings)
	bool bY = true;

protected:
	
	UPROPERTY()
	const USSGamepadUserSettings* GamepadSettings;
	
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
