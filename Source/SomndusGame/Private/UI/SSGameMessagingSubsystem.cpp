/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSGameMessagingSubsystem.h"

#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include "UI/SSCommonUIFunctionLibrary.h"
#include "UI/SSCommonUITypes.h"
#include "UI/Dialog/SSLoadingModal.h"

#define LOCTEXT_NAMESPACE "FSomndusGameModule"

class FSubsystemCollectionBase;

void USSGameMessagingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ConfirmationDialogClassPtr = ConfirmationDialogClass.LoadSynchronous();
	ErrorDialogClassPtr = ErrorDialogClass.LoadSynchronous();
	AlertDialogClassPtr = AlertDialogClass.LoadSynchronous();
	LoadingDialogClassPtr = LoadingDialogClass.LoadSynchronous();
	NotificationManager = NewObject<USSGameNotificationManager>(this);
}

void USSGameMessagingSubsystem::ShowConfirmation(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback)
{
	if (UCommonLocalPlayer* LocalPlayer = GetLocalPlayer<UCommonLocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_SS_LAYER_MODAL, ConfirmationDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
				Dialog.SetupDialog(DialogDescriptor, ResultCallback);
			});
		}
	}
}

void USSGameMessagingSubsystem::ShowError(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback)
{
	if (UCommonLocalPlayer* LocalPlayer = GetLocalPlayer<UCommonLocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_SS_LAYER_MODAL, ErrorDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
				Dialog.SetupDialog(DialogDescriptor, ResultCallback);
			});
		}
	}
}

void USSGameMessagingSubsystem::ShowAlert(UCommonGameDialogDescriptor* DialogDescriptor,
	FCommonMessagingResultDelegate ResultCallback)
{
	if (UCommonLocalPlayer* LocalPlayer = GetLocalPlayer<UCommonLocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_SS_LAYER_MODAL, AlertDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
				Dialog.SetupDialog(DialogDescriptor, ResultCallback);
			});
		}
	}
}

void USSGameMessagingSubsystem::ShowLoading(UCommonGameDialogDescriptor* DialogDescriptor,
	FCommonMessagingResultDelegate ResultCallback)
{
	if (UCommonLocalPlayer* LocalPlayer = GetLocalPlayer<UCommonLocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			CurrentLoadingDialogWidget = RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_SS_LAYER_MODAL, LoadingDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
				Dialog.SetupDialog(DialogDescriptor, ResultCallback);
			});
		}
	}
}

void USSGameMessagingSubsystem::HandleGlobalLoadingResult(ECommonMessagingResult CommonMessagingResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Global Loading dialog closed"));
}

void USSGameMessagingSubsystem::InternalShowGlobalLoading(FText Message)
{
	auto* Descriptor = CreateLoadingDescriptor(Message);
	GlobalLoadingResultCallback = FCommonMessagingResultDelegate::CreateUObject(this, &USSGameMessagingSubsystem::HandleGlobalLoadingResult);

	ShowLoading(Descriptor, GlobalLoadingResultCallback);
}

void USSGameMessagingSubsystem::ShowGlobalLoading(UObject* InWorldContextObject, FText Message)
{
	if (auto* LocalPlayer = USSCommonUIFunctionLibrary::GetLocalPlayerFromContext(InWorldContextObject))
	{
		if (auto* GameMessaging = LocalPlayer->GetSubsystem<USSGameMessagingSubsystem>())
		{
			GameMessaging->InternalShowGlobalLoading(Message);
		}
	}
}

void USSGameMessagingSubsystem::CloseCurrentLoadingDialog(UObject* InWorldContextObject)
{
	if (auto* LocalPlayer = USSCommonUIFunctionLibrary::GetLocalPlayerFromContext(InWorldContextObject))
	{
		if (auto* GameMessaging = LocalPlayer->GetSubsystem<USSGameMessagingSubsystem>())
		{
			GameMessaging->InteralCloseCurrentLoadingDialog();
		}
	}
}

void USSGameMessagingSubsystem::InteralCloseCurrentLoadingDialog()
{
	if (IsValid(CurrentLoadingDialogWidget))
	{
		CastChecked<USSLoadingModal>(CurrentLoadingDialogWidget)->CloseLoadingWindow(ECommonMessagingResult::Unknown);
	}
}

UCommonGameDialogDescriptor* USSGameMessagingSubsystem::CreateAlertDescriptor(const FText& Header, const FText& Body)
{
	UCommonGameDialogDescriptor* Descriptor = NewObject<UCommonGameDialogDescriptor>();
	Descriptor->Header = Header;
	Descriptor->Body = Body;

	FConfirmationDialogAction OkAction;
	OkAction.Result = ECommonMessagingResult::Confirmed;
	OkAction.OptionalDisplayText = LOCTEXT("Ok", "Ok");

	Descriptor->ButtonActions.Add(OkAction);

	return Descriptor;
}

UCommonGameDialogDescriptor* USSGameMessagingSubsystem::CreateLoadingDescriptor(const FText& Body)
{
	UCommonGameDialogDescriptor* Descriptor = NewObject<UCommonGameDialogDescriptor>();
	Descriptor->Header = LOCTEXT("Loading", "Loading");;
	Descriptor->Body = Body;

	return Descriptor;
}

USSGameNotificationManager* USSGameMessagingSubsystem::GetNotificationManager() const
{
	return NotificationManager;
}

#undef LOCTEXT_NAMESPACE
