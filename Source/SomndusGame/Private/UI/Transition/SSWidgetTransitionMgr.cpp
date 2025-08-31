/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Transition/SSWidgetTransitionMgr.h"

#include "Kismet/GameplayStatics.h"

void USSWidgetTransitionMgr::CreateWidgetTransition(TSubclassOf<USSWidgetTransition> Class)
{
	// Clear previous
	if (CurrentWidgetTransition)
	{
		CurrentWidgetTransition->OnFinished.RemoveDynamic(this, &USSWidgetTransitionMgr::NativeOnTransitionFinished);
		CurrentWidgetTransition->RemoveFromParent();
		CurrentWidgetTransition = nullptr;
	}

	// Get player
	auto* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	
	CurrentWidgetTransition = CreateWidget<USSWidgetTransition>(PlayerController, Class);
	if (CurrentWidgetTransition)
	{
		CurrentWidgetTransition->OnFinished.AddUniqueDynamic(this, &USSWidgetTransitionMgr::NativeOnTransitionFinished);
		CurrentWidgetTransition->AddToViewport();
	}
}

bool USSWidgetTransitionMgr::CheckWidgetTransition(TSubclassOf<USSWidgetTransition> Class)
{
	if (!Class)
	{
		return false;
	}

	// if first time create a transition widget
	if (!CurrentWidgetTransition)
	{
		CreateWidgetTransition(Class);
		return true;
	}

	// if a widget already created but not the good class -> clear and create new one
	if (!CurrentWidgetTransition->IsA(Class))
	{
		CreateWidgetTransition(Class);
		return true;
	}

	return true;
}

bool USSWidgetTransitionMgr::PlayTransition(TSubclassOf<USSWidgetTransition> Class, const FSSTransitionParams& FadeParams)
{
	if (!CheckWidgetTransition(Class))
	{
		return false;
	}
	GetCurrentWidgetTransition()->StartFade(FadeParams);
	return true;
}

bool USSWidgetTransitionMgr::FadeInTransition(TSubclassOf<USSWidgetTransition> Class, const FSSTransitionParams& FadeParams)
{
	if (!CheckWidgetTransition(Class))
	{
		return false;
	}
	GetCurrentWidgetTransition()->StartFadeIn(FadeParams);
	return true;
}

bool USSWidgetTransitionMgr::FadeOutTransition(TSubclassOf<USSWidgetTransition> Class, const FSSTransitionParams& FadeParams)
{
	if (!CheckWidgetTransition(Class))
	{
		return false;
	}
	GetCurrentWidgetTransition()->StartFadeOut(FadeParams);
	return true;
}

void USSWidgetTransitionMgr::NativeOnTransitionFinished(USSWidgetTransition* WidgetTransition)
{
	OnTransitionFinished.Broadcast(this);
}
