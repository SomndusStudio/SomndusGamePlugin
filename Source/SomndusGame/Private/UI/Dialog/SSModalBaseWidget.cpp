/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/Dialog/SSModalBaseWidget.h"

#include "UI/SSCommonUIFunctionLibrary.h"

USSModalBaseWidget::USSModalBaseWidget()
{
	// -- override default value
	bIsModal = true;
	bUseAutoShowAnimation = true;

	InputConfig = ESSWidgetInputMode::Menu;
	
	// Should collapse when deactivate
	DeactivatedVisibility = ESlateVisibility::Collapsed;
}

void USSModalBaseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Common navigation rule to lock gamepad navigation outside the modal
	USSCommonUIFunctionLibrary::CommonModalNavigationRules(OverlayModalRoot);
}

void USSModalBaseWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void USSModalBaseWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}
