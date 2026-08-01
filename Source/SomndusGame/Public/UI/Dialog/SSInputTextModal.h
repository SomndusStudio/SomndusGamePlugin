/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonRichTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Messaging/CommonGameDialog.h"
#include "SSInputTextModal.generated.h"

/**
 * Descriptor used to configure an input text modal.
 *
 * Extends the base dialog descriptor (Header / Body / ButtonActions) with the
 * extra data needed by a text prompt: the default value pre-filled in the field
 * and the hint text displayed when the field is empty.
 */
UCLASS(BlueprintType)
class SOMNDUSGAME_API USSInputTextDialogDescriptor : public UCommonGameDialogDescriptor
{
	GENERATED_BODY()

public:
	/** Text pre-filled inside the editable field when the modal opens. */
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FText DefaultText;

	/** Hint text displayed while the field is empty. */
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FText HintText;
};

/** Native delegate broadcasting the messaging result along with the typed value. */
DECLARE_DELEGATE_TwoParams(FSSInputTextResultDelegate, ECommonMessagingResult /*Result*/, const FText& /*Text*/);

/**
 * Modal that lets the player type a single line of text (name, title, etc.) and
 * confirm or cancel. Buttons are generated from the descriptor actions inside a
 * UDynamicEntryBox, exactly like USSConfirmationModal, and the typed value is
 * returned through the native result delegate.
 */
UCLASS()
class SOMNDUSGAME_API USSInputTextModal : public UCommonGameDialog
{
	GENERATED_BODY()

public:
	virtual void SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback) override;
	virtual void KillDialog() override;

	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	/** Sets the callback invoked when the modal is closed, carrying the typed text. */
	void SetOnInputTextResult(const FSSInputTextResultDelegate& InDelegate) { OnInputResultCallback = InDelegate; }

	/** Returns the current content of the input field. */
	UFUNCTION(BlueprintPure, Category = "Input")
	FText GetInputText() const;

protected:
	virtual void CloseInputWindow(ECommonMessagingResult Result);

	/** Confirms the modal when the field is committed with Enter. */
	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	FSSInputTextResultDelegate OnInputResultCallback;

	/** Result sent when the field is validated with Enter. */
	ECommonMessagingResult ConfirmResult = ECommonMessagingResult::Confirmed;

public:
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> EntryBox_Buttons;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UEditableTextBox> InputText_Value;

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Title;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UCommonRichTextBlock> RichText_Description;
};
