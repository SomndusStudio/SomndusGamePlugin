/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSGamepadLayoutWidget.generated.h"

class UCanvasPanel;
class UInputMappingContext;
class USSGamepadButtonCaptureWidget;

/**
 * Widget responsible for visually representing the layout of a gamepad,
 * and dynamically assigning input actions to button slots.
 *
 * This class scans for child button widgets (USSGamepadButtonCaptureWidget),
 * and maps input actions to their respective keys based on provided input mapping contexts.
 * 
 * Typical usage:
 * - Call UpdateFromMappingContexts to apply new input bindings.
 * - The widget will automatically match keys to buttons and update their visuals.
 */
UCLASS()
class SOMNDUSGAME_API USSGamepadLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Default mapping contexts used to populate the layout on initialization. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FName GamepadName;

	/** Cached reference to the input mapping contexts currently applied. */
	UPROPERTY(BlueprintReadOnly, Category="Config")
	TArray<TObjectPtr<UInputMappingContext>> MappingContexts;

	/** Mapping of input keys to their respective button capture widgets. */
	UPROPERTY(BlueprintReadWrite)
	TMap<FKey, USSGamepadButtonCaptureWidget*> GamepadButtonWidgets;

protected:
	virtual void NativeOnInitialized() override;

	/** Panel containing all the gamepad button widgets (usually a CanvasPanel). */
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCanvasPanel* ButtonsLayoutPanel;

public:
	
	/**
	 * Updates the layout based on the provided input mapping contexts.
	 * Each context is parsed and input actions are assigned to the correct button widget.
	 *
	 * @param InInputMappingContexts List of input mapping contexts to use for populating the layout.
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateFromMappingContexts(const TArray<UInputMappingContext*>& InInputMappingContexts);
	
	/**
	 * Searches the layout panel for all gamepad button capture widgets and caches them by key.
	 *
	 * @return true if the capture was successful, false if no layout panel or widgets were found.
	 */
	UFUNCTION(BlueprintCallable)
	bool CaptureGameButtonWidgets();

	/**
	 * Retrieves the button widget associated with a specific input key.
	 *
	 * @param InKey The key to look up.
	 * @return The corresponding capture widget, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable)
	USSGamepadButtonCaptureWidget* GetButtonWidgetFromKey(const FKey& InKey);
};
