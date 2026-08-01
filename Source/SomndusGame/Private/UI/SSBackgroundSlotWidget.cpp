/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSBackgroundSlotWidget.h"

void USSBackgroundSlotWidget::SetSelected(bool InSelected)
{
	//if (bSelected == InSelected) return;
	bSelected = InSelected;
	OnSelectedChange(bSelected);
}

void USSBackgroundSlotWidget::OnHoverAnimationFinished()
{
	if (bIsHovering && HoverLoopAnimation)
	{
		PlayAnimation(HoverLoopAnimation, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	}
}

void USSBackgroundSlotWidget::OnSelectedChange_Implementation(bool bInSelected)
{
	if (bSelected)
	{
		if (!bIsHovering)
		{
			PlayAnimationForward(HoverAnimation);
		}
	}
	else
	{
		PlayAnimationReverse(HoverAnimation);
	}
}

void USSBackgroundSlotWidget::OnHoverActive_Implementation(bool InActive)
{
	bIsHovering = InActive;

	// Todo : Add possibility to choice if we can do different anim for selected state
	if (bSelected)
	{
		if (InActive)
		{
			if (IsAnimationPlaying(HoverAnimation)) return;
		}

		return;
	}
	
	if (InActive)
	{
		if (HoverAnimation)
		{
			// Rebind à chaque fois pour éviter les doublons
			UnbindAllFromAnimationFinished(HoverAnimation);

			FWidgetAnimationDynamicEvent Delegate;
			Delegate.BindDynamic(this, &USSBackgroundSlotWidget::OnHoverAnimationFinished);
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
