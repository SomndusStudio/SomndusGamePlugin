// ©2025 Schartier Isaac. All rights reserved.


#include "UI/SSBackgroundSlotWidget.h"

void USSBackgroundSlotWidget::SetSelected(bool InSelected)
{
	if (bSelected == InSelected) return;
	
	bSelected = InSelected;
	OnSelectedChange(bSelected);
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
		return;
	}
	
	if (InActive)
	{
		PlayAnimationForward(HoverAnimation);
	}
	else
	{
		PlayAnimationReverse(HoverAnimation);
	}
}
