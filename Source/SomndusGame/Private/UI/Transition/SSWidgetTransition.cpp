// Copyright (C) 2020-2023 Schartier Isaac


#include "UI/Transition/SSWidgetTransition.h"

void USSWidgetTransition::StartFadeIn_Implementation(const FSSTransitionParams& Params)
{
}

void USSWidgetTransition::StartFadeOut_Implementation(const FSSTransitionParams& Params)
{
}

void USSWidgetTransition::StartFade_Implementation(const FSSTransitionParams& Params)
{
}

void USSWidgetTransition::NotifyFinished()
{
	OnFinished.Broadcast(this);
}
