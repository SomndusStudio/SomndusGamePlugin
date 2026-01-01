/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


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
