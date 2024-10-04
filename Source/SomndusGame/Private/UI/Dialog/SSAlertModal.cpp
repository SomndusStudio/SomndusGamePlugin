/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Dialog/SSAlertModal.h"

#include "UI/Button/SSButtonText.h"

void USSAlertModal::SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback)
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
		Button->OnClicked().AddUObject(this, &ThisClass::CloseAlertWindow, Action.Result);
		Button->SetButtonText(Action.OptionalDisplayText);
	}

	OnResultCallback = ResultCallback;
}

void USSAlertModal::KillDialog()
{
	Super::KillDialog();
}

TOptional<FUIInputConfig> USSAlertModal::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}

void USSAlertModal::CloseAlertWindow(ECommonMessagingResult Result)
{
	DeactivateWidget();
	OnResultCallback.ExecuteIfBound(Result);
}
