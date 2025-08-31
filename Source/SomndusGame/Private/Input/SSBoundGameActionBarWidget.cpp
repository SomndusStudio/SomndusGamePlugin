// Copyright (C) 2020-2023 Schartier Isaac


#include "Input/SSBoundGameActionBarWidget.h"

void USSBoundGameActionBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (bPreConstructActions)
	{
		SetupGameActions();
	}
}

void USSBoundGameActionBarWidget::SetupGameActions()
{
	if (!BoundGameActionBar)
	{
		return;
	}
	BoundGameActionBar->SetBoundActions(GameActions);
}
