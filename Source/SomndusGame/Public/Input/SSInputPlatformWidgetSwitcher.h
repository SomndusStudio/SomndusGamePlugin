/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonUserWidget.h"
#include "SSInputPlatformWidgetSet.h"
#include "Engine/StreamableManager.h"
#include "SSInputPlatformWidgetSwitcher.generated.h"

class UWidgetSwitcher;

/**
 * USSInputPlatformWidgetSwitcher
 *
 * A widget switcher responsible for displaying different gamepad-specific UI layouts
 * based on the connected input hardware. It listens for input device and method changes,
 * loads corresponding widgets asynchronously, and switches UI views accordingly.
 *
 * Responsibilities:
 * - Detect input device/method changes (keyboard, gamepad, etc.)
 * - Load platform-specific widgets for different gamepads (e.g., Xbox, PlayStation)
 * - Handle async loading and UI input blocking during widget transition
 * - Cache loaded widgets for reuse
 *
 * Used in conjunction with CommonInput and input platform settings.
 */
UCLASS()
class SOMNDUSGAME_API USSInputPlatformWidgetSwitcher : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	/** Platform input settings used to determine available gamepad widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TObjectPtr<USSInputPlatformWidgetSet> PlatformInputSet;

	/** Whether to block UI input while loading widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bBlockInputWhenLoading = true;

	/** Gamepad names currently being loaded asynchronously. */
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> LoadingGamepadNames;

	/** Widgets currently loaded and mapped by gamepad name. */
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, UUserWidget*> GamepadWidgets;

protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	/** Binds input hardware and input method change events. */
	void BindEvents();
	
	/** Resolves and applies the default gamepad layout at startup. */
	void ResolveDefaultGamepad();
	
	UFUNCTION()
	void OnInputHardwareDeviceChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	UFUNCTION()
	void HandleInputMethodChanged(ECommonInputType CommonInput);

	/** Widget switcher managing different gamepad UI layouts. */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* GamepadModeSwitcher;

	// For tracking/canceling loads
	TMap<FName, TSharedPtr<FStreamableHandle>> ActiveLoadHandles;
	
public:

	/** Refreshes the current layout using the current gamepad name. */
	void NativeRefreshLayoutType();

	/** Refreshes the layout using a specific gamepad name. */
	void NativeRefreshLayoutTypeFromName(const FName& GamepadName);

	/** Blueprint native event: toggles visibility of a loading indicator. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RequestShowLoading(bool bShow=true);

	/** Returns the already loaded widget for the specified gamepad name, if any. */
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetGamepadWidgetFromName(const FName& InGamepadName);

	/** Returns true if the widget for the specified gamepad should be loaded. */
	UFUNCTION(BlueprintCallable)
	bool ShouldLoadGamepadWidget(const FName& InGamepadName);
	
	/**
	 * Called when async widget loading finishes successfully.
	 * Instantiates and activates the loaded widget.
	 */
	UFUNCTION()
	void OnGamepadWidgetLoaded(FName Name);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGamepadWidgetLoaded(FName Name);

	/**
	 * Called if widget loading fails.
	 * Handles input resumption and error feedback.
	 */
	void OnGamepadWidgetLoaded_Fail(FName Name);

	/**
	 * Starts loading the widget associated with a gamepad name asynchronously.
	 * Returns true if a valid widget was found and loading started.
	 */
	UFUNCTION(BlueprintCallable)
	bool StartLoadingGamepadWidget(const FName& InGamepadName);
};
