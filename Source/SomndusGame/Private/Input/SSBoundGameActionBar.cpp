// Copyright (C) 2020-2023 Schartier Isaac


#include "Input/SSBoundGameActionBar.h"

#include "Input/SSGameInputActionWrapperWidget.h"

void USSBoundGameActionBar::Initialize()
{
	RebuildActionWidgets();
}

void USSBoundGameActionBar::SetBoundActions(const TArray<FSSBoundGameActionInfo>& InActions)
{
	BoundActions = InActions;
	RebuildActionWidgets();
}

void USSBoundGameActionBar::RebuildActionWidgets()
{
	ResetInternal();

	TSet<UInputAction*> UniqueActions;

	for (const FSSBoundGameActionInfo& ActionInfo : BoundActions)
	{
		if (!ActionInfo.InputAction) continue;

		if (bIgnoreDuplicateActions && UniqueActions.Contains(ActionInfo.InputAction))
		{
			continue;
		}
		
		if (bIgnoreDuplicateActions)
		{
			UniqueActions.Add(ActionInfo.InputAction);
		}

		if (!InputActionWidgetClass) continue;

		USSGameInputActionWrapperWidget* NewWidget = Cast<USSGameInputActionWrapperWidget>(CreateEntryInternal(InputActionWidgetClass));
		if (NewWidget)
		{
			NewWidget->SetInputAction(ActionInfo.InputAction);
			
			if (ActionInfo.bOverrideInputActionDisplayName)
			{
				NewWidget->SetDisplayName(ActionInfo.DisplayName);
			}
			else
			{
				NewWidget->SetDisplayName(ActionInfo.InputAction->ActionDescription);
			}
		}
	}
}