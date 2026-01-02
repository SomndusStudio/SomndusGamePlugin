/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSSubUserSettings.h"
#include "SSMouseKeyboardUserSettings.generated.h"

/**
 * Represents user settings for mouse and keyboard input within the game.
 * This class allows configuration of mouse sensitivity (horizontal and vertical)
 * and whether the mouse movement is inverted.
 */
UCLASS(config=GameUserSettings, configdonotcheckdefaults, DisplayName="Mouse/Keyboard User Settings", Category="SS|User Settings")
class SOMNDUSGAME_API USSMouseKeyboardUserSettings : public USSSubUserSettings
{
	GENERATED_BODY()

public:
	
	virtual void SetToDefaults() override;
	
	// Mouse Sensitivity
private:
	UPROPERTY(Config)
	float MouseSensitivity;

	UPROPERTY(Config)
	bool bInvertMouse;

	UPROPERTY(Config)
	bool bInvertMouseX;
	
	/** Holds the mouse horizontal sensitivity */
	UPROPERTY(Config)
	double MouseSensitivityX;

	/** Holds the mouse vertical sensitivity */
	UPROPERTY(Config)
	double MouseSensitivityY;
	
public:

	void ApplyInputSensitivity();
	
	UFUNCTION(BlueprintPure, Category = "Settings|Mouse")
	float GetMouseSensitivity() const { return MouseSensitivity; };

	UFUNCTION(BlueprintCallable, Category = "Settings|Mouse")
	void SetMouseSensitivity(float NewValue) { ChangeValueAndDirty(MouseSensitivity, NewValue); ApplyInputSensitivity();}

	UFUNCTION(BlueprintPure, Category = "Settings|Mouse")
	bool GetInvertMouse() const { return bInvertMouse; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Mouse")
	void SetInvertMouse(bool NewValue) { ChangeValueAndDirty(bInvertMouse, NewValue);}

	UFUNCTION(BlueprintPure, Category = "Settings|Mouse")
	bool GetInvertMouseX() const { return bInvertMouseX; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Mouse")
	void SetInvertMouseX(bool NewValue) { ChangeValueAndDirty(bInvertMouseX, NewValue);}
	
	UFUNCTION(BlueprintPure)
	double GetMouseSensitivityX() const { return MouseSensitivityX; }
	UFUNCTION()
	void SetMouseSensitivityX(double NewValue) { ChangeValueAndDirty(MouseSensitivityX, NewValue); ApplyInputSensitivity(); }

	UFUNCTION(BlueprintPure)
	double GetMouseSensitivityY() const { return MouseSensitivityY; }
	
	UFUNCTION()
	void SetMouseSensitivityY(double NewValue) { ChangeValueAndDirty(MouseSensitivityY, NewValue); ApplyInputSensitivity(); }

};
