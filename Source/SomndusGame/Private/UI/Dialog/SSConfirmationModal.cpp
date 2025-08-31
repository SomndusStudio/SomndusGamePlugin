/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Dialog/SSConfirmationModal.h"

#include "SSLog.h"
#include "UI/Button/SSButtonText.h"


void USSConfirmationModal::SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback)
{
	if (!Descriptor)
	{
		UE_LOG(LogSomndusGame, Error, TEXT("[%s] SetupDialog failed: Descriptor is null."), *GetNameSafe(this));
		return;
	}

	if (!Text_Title || !RichText_Description || !EntryBox_Buttons)
	{
		UE_LOG(LogSomndusGame, Error, TEXT("[%s] SetupDialog failed: One or more UI components are null."), *GetNameSafe(this));
		return;
	}

	// Call base class implementation
	Super::SetupDialog(Descriptor, ResultCallback);

	// Set dialog title and description
	Text_Title->SetText(Descriptor->Header);
	RichText_Description->SetText(Descriptor->Body);

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

		Button->OnClicked().AddUObject(this, &ThisClass::CloseConfirmationWindow, Action.Result);
		Button->SetButtonText(Action.OptionalDisplayText);
	}

	// Store the result callback for later use
	OnResultCallback = ResultCallback;
	
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] SetupDialog completed successfully with %d actions."), *GetNameSafe(this), Descriptor->ButtonActions.Num());
}

void USSConfirmationModal::KillDialog()
{
	Super::KillDialog();
}

UWidget* USSConfirmationModal::NativeGetDesiredFocusTarget() const
{
	if (EntryBox_Buttons->GetAllEntries().IsValidIndex(0))
	{
		return EntryBox_Buttons->GetAllEntries()[0];
	}
	return Super::NativeGetDesiredFocusTarget();
}

TOptional<FUIInputConfig> USSConfirmationModal::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}

void USSConfirmationModal::CloseConfirmationWindow(ECommonMessagingResult Result)
{
	DeactivateWidget();
	OnResultCallback.ExecuteIfBound(Result);
}

