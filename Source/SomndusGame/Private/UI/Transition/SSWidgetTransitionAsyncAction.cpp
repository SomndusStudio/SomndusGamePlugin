// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Transition/SSWidgetTransitionAsyncAction.h"

#include "Kismet/GameplayStatics.h"
#include "UI/SSGameUIManagerSubsystem.h"
#include "UI/Transition/SSWidgetTransitionMgr.h"

void USSWidgetTransitionAsyncAction::Activate()
{
	auto* GameUIManagerSubsystem = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<
		USSGameUIManagerSubsystem>();
	check(GameUIManagerSubsystem);

	auto* WidgetTransitionMgr = GameUIManagerSubsystem->GetWidgetTransitionMgr();

	// bind when fade finish
	WidgetTransitionMgr->OnTransitionFinished.AddUniqueDynamic(
		this, &USSWidgetTransitionAsyncAction::NativeOnTransitionFinished);
	switch (FadeType)
	{
	case 0:
		WidgetTransitionMgr->PlayTransition(WidgetTransitionClass);
		break;
	case 1:
		WidgetTransitionMgr->FadeInTransition(WidgetTransitionClass);
		break;
	case 2:
		WidgetTransitionMgr->FadeOutTransition(WidgetTransitionClass);
		break;
	default: WidgetTransitionMgr->PlayTransition(WidgetTransitionClass);
		break;
	}
}

USSWidgetTransitionAsyncAction* USSWidgetTransitionAsyncAction::AsyncFade(
	UObject* WorldContextObject, TSubclassOf<USSWidgetTransition> InWidgetTransitionClass,
	const FSSTransitionParams& FadeParams, const int32 ZOrder)
{
	USSWidgetTransitionAsyncAction* AsyncAction = NewObject<USSWidgetTransitionAsyncAction>();

	AsyncAction->RegisterWithGameInstance(WorldContextObject);
	AsyncAction->WorldContextObject = WorldContextObject;
	AsyncAction->WidgetTransitionClass = InWidgetTransitionClass;
	AsyncAction->FadeParams = FadeParams;

	AsyncAction->OwningPlayer = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	AsyncAction->ZOrder = ZOrder;
	AsyncAction->FadeType = 0;

	return AsyncAction;
}

USSWidgetTransitionAsyncAction* USSWidgetTransitionAsyncAction::AsyncFadeIn(
	UObject* WorldContextObject, TSubclassOf<USSWidgetTransition> InWidgetTransitionClass,
	const FSSTransitionParams& FadeParams, const int32 ZOrder)
{
	auto* AsyncAction = AsyncFade(WorldContextObject, InWidgetTransitionClass, FadeParams, ZOrder);
	AsyncAction->FadeType = 1;
	return AsyncAction;
}

USSWidgetTransitionAsyncAction* USSWidgetTransitionAsyncAction::AsyncFadeOut(UObject* WorldContextObject,
                                                                             TSubclassOf<USSWidgetTransition>
                                                                             InWidgetTransitionClass,
                                                                             const FSSTransitionParams& FadeParams,
                                                                             const int32 ZOrder)
{
	auto* AsyncAction = AsyncFade(WorldContextObject, InWidgetTransitionClass, FadeParams, ZOrder);
	AsyncAction->FadeType = 2;
	return AsyncAction;
}

void USSWidgetTransitionAsyncAction::NativeOnTransitionFinished(USSWidgetTransitionMgr* WidgetTransitionMgr)
{
	// clear bind
	WidgetTransitionMgr->OnTransitionFinished.RemoveDynamic(
		this, &USSWidgetTransitionAsyncAction::NativeOnTransitionFinished);

	Finished.Broadcast();
	SetReadyToDestroy();
}
