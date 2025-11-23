/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSGameMessagingSubsystem.h"

#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include "SSLog.h"
#include "UI/SSCommonUIFunctionLibrary.h"
#include "UI/SSCommonUITypes.h"
#include "UI/Dialog/SSLoadingModal.h"

#define LOCTEXT_NAMESPACE "FSomndusGameModule"

class FSubsystemCollectionBase;

namespace SSGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Tag_Loading_Modal, "Loading.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Tag_Loading_Indicator, "Loading.Indicator");
}

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
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(SSGameplayTags::TAG_SS_LAYER_MODAL, ConfirmationDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
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
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(SSGameplayTags::TAG_SS_LAYER_MODAL, ErrorDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
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
			RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(SSGameplayTags::TAG_SS_LAYER_MODAL, AlertDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
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
			CurrentLoadingDialogWidget = RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(SSGameplayTags::TAG_SS_LAYER_MODAL, LoadingDialogClassPtr, [DialogDescriptor, ResultCallback](UCommonGameDialog& Dialog) {
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
	Descriptor->Header = LOCTEXT("Loading", "Loading");
	Descriptor->Body = Body;

	return Descriptor;
}

FName USSGameMessagingSubsystem::RequestLoadingInformation(const FText& Message, FGameplayTag LoadingToken, FGameplayTag LoadingType)
{
	// Token fallback
	if (!LoadingToken.IsValid())
	{
		LoadingToken = SSGameplayTags::Tag_Loading_Modal;
		UE_LOG(LogSomndusGame, Warning, TEXT("System: Loading token is invalid, fallback to default token"));
	}

	// Generate un FName token + timestamp (ou UUID)
	FString GeneratedTokenString = FString::Printf(TEXT("%s_%s"), 
		*LoadingToken.GetTagName().ToString(), 
		*FGuid::NewGuid().ToString());
	
	FName GeneratedToken = FName(*GeneratedTokenString);
	
	UE_LOG(LogSomndusGame, Log, TEXT("LoadingInformation: Message -> %s"), *Message.ToString());

	// Store token
	FSSLoadingInformationTokenInfo TokenInfo;
	TokenInfo.NameToken = GeneratedToken;
	TokenInfo.Message = Message;
	TokenInfo.Token = LoadingToken;
	TokenInfo.LoadingType = LoadingType;

	AddUniqueLoadingInfo(TokenInfo);

	OnLoadingTokenChange.Broadcast(this, LoadingType, false);
	
	return GeneratedToken;
}

void USSGameMessagingSubsystem::StopLoadingInformation(FGameplayTag LoadingToken, FGameplayTag LoadingType)
{
	// Token fallback
	if (!LoadingToken.IsValid())
	{
		LoadingToken = SSGameplayTags::Tag_Loading_Modal;
		UE_LOG(LogSomndusGame, Warning, TEXT("System: Loading token is invalid, fallback to default token"));
	}

	UE_LOG(LogSomndusGame, Log, TEXT("LoadingInformation: Stopped !"));

	RemoveLoadingInfoByToken(LoadingToken);

	OnLoadingTokenChange.Broadcast(this, LoadingType, true);
}

void USSGameMessagingSubsystem::AddUniqueLoadingInfo(const FSSLoadingInformationTokenInfo& NewInfo)
{
	// Check if already exist
	bool bExists = LoadingInformations.ContainsByPredicate([&](const FSSLoadingInformationTokenInfo& Info)
	{
		return Info.Token == NewInfo.Token;
	});

	LoadingInformations.Add(NewInfo);
}

void USSGameMessagingSubsystem::RemoveLoadingInfoByToken(const FGameplayTag& TokenToRemove)
{
	int32 RemovedToken = LoadingInformations.RemoveAll([TokenToRemove](const FSSLoadingInformationTokenInfo& Info)
	{
		return Info.Token == TokenToRemove;
	});

	UE_LOG(LogSomndusGame, Log, TEXT("LoadingInformation: Removed %d token"), RemovedToken);
}

bool USSGameMessagingSubsystem::FindBetterLoadingInfo(FGameplayTag LoadingType, FSSLoadingInformationTokenInfo& OutResult)
{
	for (const auto& LoadingInfo : LoadingInformations)
	{
		if (LoadingInfo.LoadingType == LoadingType)
		{
			OutResult = LoadingInfo;
			return true;
		}
	}
	return false;
}

bool USSGameMessagingSubsystem::ContainAnyLoadingInfoFromType(FGameplayTag LoadingType)
{
	for (const auto& LoadingInfo : LoadingInformations)
	{
		if (LoadingInfo.LoadingType == LoadingType)
		{
			return true;
		}
	}
	return false;
}


USSGameNotificationManager* USSGameMessagingSubsystem::GetNotificationManager() const
{
	return NotificationManager;
}

#undef LOCTEXT_NAMESPACE
