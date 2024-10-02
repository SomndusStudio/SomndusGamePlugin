// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialog/SSConfirmationModal.h"

#include "UI/Button/SSButtonText.h"


void USSConfirmationModal::SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback)
{
	Super::SetupDialog(Descriptor, ResultCallback);

	Text_Title->SetText(Descriptor->Header);
	RichText_Description->SetText(Descriptor->Body);

	EntryBox_Buttons->Reset<USSButtonText>([](USSButtonText& Button)
	{
		Button.OnClicked().Clear();
	});

	for (const FConfirmationDialogAction& Action : Descriptor->ButtonActions)
	{
		USSButtonText* Button = EntryBox_Buttons->CreateEntry<USSButtonText>();
		Button->OnClicked().AddUObject(this, &ThisClass::CloseConfirmationWindow, Action.Result);
		Button->SetButtonText(Action.OptionalDisplayText);
	}

	OnResultCallback = ResultCallback;
}

void USSConfirmationModal::KillDialog()
{
	Super::KillDialog();
}

void USSConfirmationModal::CloseConfirmationWindow(ECommonMessagingResult Result)
{
	DeactivateWidget();
	OnResultCallback.ExecuteIfBound(Result);
}

