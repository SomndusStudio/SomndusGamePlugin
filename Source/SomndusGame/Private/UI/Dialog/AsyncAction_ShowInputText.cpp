/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Dialog/AsyncAction_ShowInputText.h"

#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "Messaging/CommonMessagingSubsystem.h"
#include "UI/SSGameMessagingSubsystem.h"

#define LOCTEXT_NAMESPACE "SomndusGame"

UAsyncAction_ShowInputText* UAsyncAction_ShowInputText::ShowInputText(
	UObject* InWorldContextObject,
	TSubclassOf<USSInputTextModal> InputTextClass,
	FText Title,
	FText Message,
	FText DefaultText,
	FText HintText,
	FText ConfirmText,
	FText CancelText)
{
	UAsyncAction_ShowInputText* Action = NewObject<UAsyncAction_ShowInputText>();
	Action->WorldContextObject = InWorldContextObject;
	Action->DialogClass = InputTextClass;

	// Build the descriptor carrying the prompt data + confirm/cancel actions
	USSInputTextDialogDescriptor* NewDescriptor = NewObject<USSInputTextDialogDescriptor>();
	NewDescriptor->Header = Title;
	NewDescriptor->Body = Message;
	NewDescriptor->DefaultText = DefaultText;
	NewDescriptor->HintText = HintText;

	FConfirmationDialogAction ConfirmAction;
	ConfirmAction.Result = ECommonMessagingResult::Confirmed;
	ConfirmAction.OptionalDisplayText = ConfirmText.IsEmpty() ? LOCTEXT("Confirm", "Confirm") : ConfirmText;
	NewDescriptor->ButtonActions.Add(ConfirmAction);

	FConfirmationDialogAction CancelAction;
	CancelAction.Result = ECommonMessagingResult::Cancelled;
	CancelAction.OptionalDisplayText = CancelText.IsEmpty() ? LOCTEXT("Cancel", "Cancel") : CancelText;
	NewDescriptor->ButtonActions.Add(CancelAction);

	Action->Descriptor = NewDescriptor;
	Action->RegisterWithGameInstance(InWorldContextObject);

	return Action;
}

void UAsyncAction_ShowInputText::Activate()
{
	if (WorldContextObject && !TargetLocalPlayer)
	{
		if (UUserWidget* UserWidget = Cast<UUserWidget>(WorldContextObject))
		{
			TargetLocalPlayer = UserWidget->GetOwningLocalPlayer<ULocalPlayer>();
		}
		else if (APlayerController* PC = Cast<APlayerController>(WorldContextObject))
		{
			TargetLocalPlayer = PC->GetLocalPlayer();
		}
		else if (UWorld* World = WorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance<UGameInstance>())
			{
				TargetLocalPlayer = GameInstance->GetPrimaryPlayerController(false)->GetLocalPlayer();
			}
		}
	}

	if (TargetLocalPlayer && DialogClass)
	{
		if (USSGameMessagingSubsystem* Messaging = TargetLocalPlayer->GetSubsystem<USSGameMessagingSubsystem>())
		{
			FSSInputTextResultDelegate ResultCallback = FSSInputTextResultDelegate::CreateUObject(this, &UAsyncAction_ShowInputText::HandleInputResult);
			Messaging->ShowInputText(DialogClass, Descriptor, ResultCallback);
			return;
		}
	}

	// If we couldn't show the modal, just broadcast an unknown result with empty text
	HandleInputResult(ECommonMessagingResult::Unknown, FText::GetEmpty());
}

void UAsyncAction_ShowInputText::HandleInputResult(ECommonMessagingResult Result, const FText& Text)
{
	OnResult.Broadcast(Result, Text);

	SetReadyToDestroy();
}

#undef LOCTEXT_NAMESPACE
