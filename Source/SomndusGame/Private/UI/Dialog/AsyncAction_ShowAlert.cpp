/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Dialog/AsyncAction_ShowAlert.h"

#include "Blueprint/UserWidget.h"
#include "Messaging/CommonMessagingSubsystem.h"
#include "UI/SSGameMessagingSubsystem.h"

UAsyncAction_ShowAlert* UAsyncAction_ShowAlert::ShowAlert(UObject* InWorldContextObject, FText Title, FText Message)
{
	UAsyncAction_ShowAlert* Action = NewObject<UAsyncAction_ShowAlert>();
	Action->WorldContextObject = InWorldContextObject;
	Action->Descriptor = USSGameMessagingSubsystem::CreateAlertDescriptor(Title, Message);
	Action->RegisterWithGameInstance(InWorldContextObject);

	return Action;
}

void UAsyncAction_ShowAlert::Activate()
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

	if (TargetLocalPlayer)
	{
		if (USSGameMessagingSubsystem* Messaging = TargetLocalPlayer->GetSubsystem<USSGameMessagingSubsystem>())
		{
			FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate::CreateUObject(this, &UAsyncAction_ShowAlert::HandleAlertResult);
			Messaging->ShowAlert(Descriptor, ResultCallback);
			return;
		}
	}
	
	// If we couldn't make the confirmation, just handle an unknown result and broadcast nothing
	HandleAlertResult(ECommonMessagingResult::Unknown);
}

void UAsyncAction_ShowAlert::HandleAlertResult(ECommonMessagingResult AlertResult)
{
	OnResult.Broadcast(AlertResult);

	SetReadyToDestroy();
}
