/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Entry/SSWidgetSlotEntry_Animation.h"

#include "UI/SSBackgroundSlotWidget.h"

void USSWidgetSlotEntry_Animation::SetHoverEffect(bool bInActive)
{
	bIsHovered = bInActive;

	if (bInActive)
	{
		if (BackgroundWidget)
		{
			BackgroundWidget->OnHoverActive(true);
		}

		if (HoverAnimation)
		{
			// Rebind à chaque fois pour éviter les doublons
			UnbindAllFromAnimationFinished(HoverAnimation);

			FWidgetAnimationDynamicEvent Delegate;
			Delegate.BindDynamic(this, &USSWidgetSlotEntry_Animation::OnHoverAnimationFinished);
			BindToAnimationFinished(HoverAnimation, Delegate);

			PlayAnimationForward(HoverAnimation);
		}
		else if (HoverLoopAnimation)
		{
			// Pas d'intro : on lance la loop direct
			PlayAnimation(HoverLoopAnimation, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f);
		}
	}
	else
	{
		if (BackgroundWidget)
		{
			BackgroundWidget->OnHoverActive(false);
		}

		if (HoverLoopAnimation)
		{
			StopAnimation(HoverLoopAnimation);
		}

		if (HoverAnimation)
		{
			PlayAnimationReverse(HoverAnimation);
		}
	}
}

FReply USSWidgetSlotEntry_Animation::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	SetHoverEffect(true);
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void USSWidgetSlotEntry_Animation::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	SetHoverEffect(false);
	Super::NativeOnFocusLost(InFocusEvent);
}

void USSWidgetSlotEntry_Animation::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetHoverEffect(true);
}

void USSWidgetSlotEntry_Animation::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetHoverEffect(false);
}

void USSWidgetSlotEntry_Animation::OnHoverAnimationFinished()
{
	if (bIsHovered && HoverLoopAnimation)
	{
		PlayAnimation(HoverLoopAnimation, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	}
}

void USSWidgetSlotEntry_Animation::PlayDisappearEffect_Implementation()
{
	// Play only if SelectedConfirmAnimation not played
	if (IsAnimationPlaying(SelectedConfirmAnimation))
	{
		return;
	}
	if (DisappearAnimation)
	{
		PlayAnimationForward(DisappearAnimation);
	}
}

void USSWidgetSlotEntry_Animation::PlaySelectConfirmEffect_Implementation()
{
	if (SelectedConfirmAnimation)
	{
		PlayAnimationForward(SelectedConfirmAnimation);
	}
}
