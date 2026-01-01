/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Navigation/SSGameNavigableScreen.h"

#include "Components/WidgetSwitcher.h"
#include "UI/Navigation/SSTabMenuNavigationWidget.h"

void USSGameNavigableScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Bind top nav
	if (TopNavigationWidget && WidgetSwitcherContent)
	{
		TopNavigationWidget->OnChange.AddUniqueDynamic(this, &ThisClass::NativeOnTopNavigationChange);
	}
}

void USSGameNavigableScreen::NativeConstruct()
{
	Super::NativeConstruct();

	// Default state
	if (TopNavigationWidget && WidgetSwitcherContent)
	{
		WidgetSwitcherContent->SetActiveWidgetIndex(0);
		OnTopNavigationChange(TopNavigationWidget, 0, "");
	}
}

void USSGameNavigableScreen::NativeOnTopNavigationChange(USSTabListWidget* Widget, int32 Index, FName Info)
{
	// Default behavior it's to switch current widget state
	if (WidgetSwitcherContent)
	{
		WidgetSwitcherContent->SetActiveWidgetIndex(Index);
		
		// Overridable in bp
		OnTopNavigationChange(Cast<USSTabMenuNavigationWidget>(Widget), Index, Info);
	}
}
