// © 2024 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Components/Widget.h"
#include "SSGameInputActionWidget.generated.h"

class UCommonInputSubsystem;

/**
 * USSGameInputActionWidget is a customizable widget for displaying input actions
 * such as keyboard or gamepad bindings. It provides mechanisms to dynamically
 * update its appearance based on the current input method.
 */
UCLASS(BlueprintType, Blueprintable)
class SOMNDUSGAME_API USSGameInputActionWidget : public UWidget
{
	GENERATED_BODY()

protected:

	/**
	 * Returns the Common Input Subsystem used to determine current input type.
	 */
	UCommonInputSubsystem* GetInputSubsystem() const;
	
	/** Begin UWidget */
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	/** End UWidet */

	/**
	 * Refreshes the input action widget. Can be called from Blueprint.
	 */
	UFUNCTION(BlueprintCallable)
	void Refresh();

	/**
	 * Updates the input action display based on the current input method.
	 */
	virtual void UpdateInputActionWidget();
	
	/**
	 * Attempts to update the widget to show keyboard-specific input.
	 * 
	 * @param CommonInputSubsystem The input subsystem reference used to determine current input.
	 * @return True if keyboard input was successfully applied, false otherwise.
	 */
	virtual bool UpdateKeyboardAction(const UCommonInputSubsystem* CommonInputSubsystem);

	/**
	 * Starts or stops listening for input method changes.
	 * 
	 * @param bListen Whether to start or stop listening.
	 */
	void ListenToInputMethodChanged(bool bListen = true);

	/**
	 * Called when the input method has changed.
	 * 
	 * @param InInputType The new input type detected.
	 */
	void HandleInputMethodChanged(ECommonInputType InInputType);

	/**
	 * Determines whether the action icon should be updated.
	 * 
	 * @return True if icon needs to be updated.
	 */
	virtual bool ShouldUpdateActionWidgetIcon() const;

	/**
	 * Gets the current icon brush representing the input action.
	 * 
	 * @return The Slate brush for the icon.
	 */
	UFUNCTION(BlueprintCallable, Category = CommonActionWidget)
	virtual FSlateBrush GetIcon() const;
	
	/** Slate image for the keyboard background. */
	TSharedPtr<SImage> KeyboardBackground;

	/** Slate text block used to show keyboard key text. */
	TSharedPtr<STextBlock> KeyboardTextBlock;

	/** Default text shown for keyboard key if none is assigned. */
	FText KeyDefaultText;
	
	/** Container box for the keyboard input key UI. */
	TSharedPtr<SBox> MyKeyBox;

	/** Slate image for displaying the gamepad icon. */
	TSharedPtr<SImage> MyIcon;

	/** Current icon brush for input action. */
	FSlateBrush Icon;
	
public:

	/**
	 * Sets the input action to display on this widget.
	 * 
	 * @param InInputAction The input action to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInputAction(class UInputAction* InInputAction);
	
#if WITH_EDITORONLY_DATA
	/** Key used in editor for preview purposes. */
	UPROPERTY(EditAnywhere, Category = "Input")
	FKey DesignTimeKey;
#endif

	/** The input action associated with this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<class UInputAction> InputAction;

	/**
	 * When true, the widget uses a fixed input type rather than detecting it automatically.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	bool bForceInputType = false;
	
	/**
	 * When true, only gamepad keys will be displayed, even if keyboard is active.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	bool bGamepad = false;

	/** Brush used for the background when displaying keyboard input. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Keyboard")
	FSlateBrush KeyboardBackgroundBrush;

	/** Font style used for displaying keyboard key text. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Keyboard")
	FSlateFontInfo KeyboardFontStyle;
};
