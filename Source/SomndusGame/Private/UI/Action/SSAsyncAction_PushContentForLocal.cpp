// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Action/SSAsyncAction_PushContentForLocal.h"
#include "PrimaryGameLayout.h"
#include "Engine/StreamableManager.h"
#include "UI/SSCommonUIFunctionLibrary.h"

void USSAsyncAction_PushContentForLocal::Cancel()
{
	Super::Cancel();
	
	if (StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
		StreamingHandle.Reset();
	}
}

USSAsyncAction_PushContentForLocal* USSAsyncAction_PushContentForLocal::PushContentToLayerForLocal(
	UObject* WorldContextObject, TSoftClassPtr<UCommonActivatableWidget> InWidgetClass, FGameplayTag InLayerName,
	bool bSuspendInputUntilComplete)
{
	if (InWidgetClass.IsNull())
	{
		FFrame::KismetExecutionMessage(TEXT("PushContentToLayer was passed a null WidgetClass"), ELogVerbosity::Error);
		return nullptr;
	}
	
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (auto* OwingPlayerController = USSCommonUIFunctionLibrary::GetLocalPlayerControllerFromContext(WorldContextObject))
		{
			auto* Action = NewObject<USSAsyncAction_PushContentForLocal>();
			Action->WidgetClass = InWidgetClass;
			Action->OwningPlayerPtr = OwingPlayerController;
			Action->LayerName = InLayerName;
			Action->bSuspendInputUntilComplete = bSuspendInputUntilComplete;
			Action->RegisterWithGameInstance(World);

			return Action;
		}

		FFrame::KismetExecutionMessage(TEXT("Can't found valid OwingPlayerController was passed a null WidgetClass"), ELogVerbosity::Error);
	}

	return nullptr;
}

void USSAsyncAction_PushContentForLocal::Activate()
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayout(OwningPlayerPtr.Get()))
	{
		TWeakObjectPtr<USSAsyncAction_PushContentForLocal> WeakThis = this;
		StreamingHandle = RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(LayerName, bSuspendInputUntilComplete, WidgetClass, [this, WeakThis](EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget) {
			if (WeakThis.IsValid())
			{
				switch (State)
				{
					case EAsyncWidgetLayerState::Initialize:
						BeforePush.Broadcast(Widget);
						break;
					case EAsyncWidgetLayerState::AfterPush:
						AfterPush.Broadcast(Widget);
						SetReadyToDestroy();
						break;
					case EAsyncWidgetLayerState::Canceled:
						SetReadyToDestroy();
						break;
				}
			}
			SetReadyToDestroy();
		});
	}
	else
	{
		SetReadyToDestroy();
	}
}
