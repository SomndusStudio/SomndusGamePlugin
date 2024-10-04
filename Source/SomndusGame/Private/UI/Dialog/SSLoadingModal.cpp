/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Dialog/SSLoadingModal.h"

void USSLoadingModal::SetupDialog(UCommonGameDialogDescriptor* Descriptor,
	FCommonMessagingResultDelegate ResultCallback)
{
	Super::SetupDialog(Descriptor, ResultCallback);

	Text_Title->SetText(Descriptor->Header);
	RichText_Description->SetText(Descriptor->Body);

	OnResultCallback = ResultCallback;
}

void USSLoadingModal::KillDialog()
{
	Super::KillDialog();
}

TOptional<FUIInputConfig> USSLoadingModal::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}

void USSLoadingModal::CloseLoadingWindow(ECommonMessagingResult Result)
{
	DeactivateWidget();
	OnResultCallback.ExecuteIfBound(Result);
}
