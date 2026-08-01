/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Messaging/CommonGameDialog.h"
#include "UI/Dialog/SSInputTextModal.h"
#include "AsyncAction_ShowInputText.generated.h"

enum class ECommonMessagingResult : uint8;

/** Multicast delegate exposed to Blueprint, carrying the result and the typed text. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSSInputTextResultMCDelegate, ECommonMessagingResult, Result, const FText&, Text);

/**
 * Async action that opens a text input modal and returns the value typed by the
 * player. The modal class is passed as a parameter so several different prompt
 * widgets can be used with the same node.
 */
UCLASS()
class SOMNDUSGAME_API UAsyncAction_ShowInputText : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Show a text input modal.
	 *
	 * @param InWorldContextObject	World context (widget, player controller...).
	 * @param InputTextClass			The modal widget class to spawn for this prompt.
	 * @param Title						Header displayed at the top of the modal.
	 * @param Message					Body / description displayed above the field.
	 * @param DefaultText				Value pre-filled inside the field.
	 * @param HintText					Hint displayed while the field is empty.
	 * @param ConfirmText				Label of the confirm button.
	 * @param CancelText				Label of the cancel button.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject", AdvancedDisplay = "DefaultText,HintText,ConfirmText,CancelText"))
	static UAsyncAction_ShowInputText* ShowInputText(
		UObject* InWorldContextObject,
		TSubclassOf<USSInputTextModal> InputTextClass,
		FText Title,
		FText Message,
		FText DefaultText,
		FText HintText,
		FText ConfirmText,
		FText CancelText
	);

	virtual void Activate() override;

public:
	/** Fired once the modal is closed. Text is only meaningful when Result is Confirmed. */
	UPROPERTY(BlueprintAssignable)
	FSSInputTextResultMCDelegate OnResult;

private:
	void HandleInputResult(ECommonMessagingResult Result, const FText& Text);

	UPROPERTY(Transient)
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> TargetLocalPlayer;

	UPROPERTY(Transient)
	TSubclassOf<USSInputTextModal> DialogClass;

	UPROPERTY(Transient)
	TObjectPtr<USSInputTextDialogDescriptor> Descriptor;
};
