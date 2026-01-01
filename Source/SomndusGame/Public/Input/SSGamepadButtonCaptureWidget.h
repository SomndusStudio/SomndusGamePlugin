/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSGamepadButtonCaptureWidget.generated.h"

class UTextBlock;
class UImage;
class UInputAction;
/**
 * A widget used to display and manage a single gamepad button mapping, including primary and extra inputs.
 *
 * This widget is typically used in control settings screens to show which gamepad key is bound to a specific action,
 * and optionally display an icon representing the input.
 */
UCLASS()
class SOMNDUSGAME_API USSGamepadButtonCaptureWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool bEditable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TArray<FKey> AttachedKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FName GamepadName;

protected:
	virtual void NativeOnInitialized() override;

public:
	/** Triggers a refresh of the slot and extra slot UI (Blueprint-callable). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Refresh();

	/** Refreshes the main display slot, typically the name of the bound action. */
	void NativeRefreshSlot();
	
	/** Refreshes the display of additional actions (e.g., alternative bindings). */
	void NativeRefreshExtraSlot();

	/** Sets the primary or extra input action associated with this widget. */
	UFUNCTION(BlueprintCallable)
	void SetInputAction(const UInputAction* InInputAction);

	void ClearInputAction();
	
	/** Returns the primary attached key (the first in the AttachedKeys array). */
	FKey GetPrimaryAttachedKey() const;

protected:

	/** Image widget to display the icon corresponding to the bound key. */
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UImage* InputIconWidget;

	/** Optional text block to display the name of the input action. */
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UTextBlock* NameSlotWidget;

	/** Optional text block to display the names of extra input actions. */
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UTextBlock* ExtraNameSlotWidget;
	
public:
	
	/** The input action associated with this slot (main binding). */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction;

	/** Additional input actions associated with this slot (e.g., modifiers or alternates). */
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<const UInputAction>> ExtraInputActions;
};
