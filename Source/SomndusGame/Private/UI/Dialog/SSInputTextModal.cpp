/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Dialog/SSInputTextModal.h"

#include "SSLog.h"
#include "UI/Button/SSButtonText.h"


void USSInputTextModal::SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback)
{
	if (!Descriptor)
	{
		UE_LOG(LogSomndusGame, Error, TEXT("[%s] SetupDialog failed: Descriptor is null."), *GetNameSafe(this));
		return;
	}

	if (!Text_Title || !EntryBox_Buttons || !InputText_Value)
	{
		UE_LOG(LogSomndusGame, Error, TEXT("[%s] SetupDialog failed: One or more UI components are null."), *GetNameSafe(this));
		return;
	}

	// Call base class implementation
	Super::SetupDialog(Descriptor, ResultCallback);

	// Set dialog title and description
	Text_Title->SetText(Descriptor->Header);
	if (RichText_Description)
	{
		RichText_Description->SetText(Descriptor->Body);
	}

	// Apply input specific data when the descriptor provides it
	if (const USSInputTextDialogDescriptor* InputDescriptor = Cast<USSInputTextDialogDescriptor>(Descriptor))
	{
		InputText_Value->SetHintText(InputDescriptor->HintText);
		InputText_Value->SetText(InputDescriptor->DefaultText);
	}
	else
	{
		InputText_Value->SetText(FText::GetEmpty());
	}

	// Validate with Enter directly from the field
	InputText_Value->OnTextCommitted.RemoveAll(this);
	InputText_Value->OnTextCommitted.AddDynamic(this, &ThisClass::HandleTextCommitted);

	// Reset existing buttons and clear their delegates
	EntryBox_Buttons->Reset<USSButtonText>([](USSButtonText& Button)
	{
		Button.OnClicked().Clear();
	});

	// Create a new button for each action in the descriptor
	for (const FConfirmationDialogAction& Action : Descriptor->ButtonActions)
	{
		USSButtonText* Button = EntryBox_Buttons->CreateEntry<USSButtonText>();
		if (!Button)
		{
			UE_LOG(LogSomndusGame, Warning, TEXT("[%s] Failed to create USSButtonText entry."), *GetNameSafe(this));
			continue;
		}

		// The first confirmed action defines the result used when validating with Enter
		if (Action.Result == ECommonMessagingResult::Confirmed)
		{
			ConfirmResult = Action.Result;
		}

		Button->OnClicked().AddUObject(this, &ThisClass::CloseInputWindow, Action.Result);
		Button->SetButtonText(Action.OptionalDisplayText);
	}

	UE_LOG(LogSomndusGame, Log, TEXT("[%s] SetupDialog completed successfully with %d actions."), *GetNameSafe(this), Descriptor->ButtonActions.Num());
}

void USSInputTextModal::KillDialog()
{
	Super::KillDialog();
}

UWidget* USSInputTextModal::NativeGetDesiredFocusTarget() const
{
	// Focus the field first so the platform virtual keyboard opens right away on console
	if (InputText_Value)
	{
		return InputText_Value;
	}
	return Super::NativeGetDesiredFocusTarget();
}

TOptional<FUIInputConfig> USSInputTextModal::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}

FText USSInputTextModal::GetInputText() const
{
	return InputText_Value ? InputText_Value->GetText() : FText::GetEmpty();
}

void USSInputTextModal::HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		CloseInputWindow(ConfirmResult);
	}
}

void USSInputTextModal::CloseInputWindow(ECommonMessagingResult Result)
{
	const FText EnteredText = GetInputText();

	DeactivateWidget();

	OnInputResultCallback.ExecuteIfBound(Result, EnteredText);
}
