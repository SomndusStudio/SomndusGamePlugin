// © 2024 Schartier Isaac. All rights reserved.


#include "UI/Dialog/SSModalBaseWidget.h"

#include "UI/SSCommonUIFunctionLibrary.h"

USSModalBaseWidget::USSModalBaseWidget()
{
	// -- override default value
	bIsModal = true;

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

	if (ShowAnimation)
	{
		PlayAnimationForward(ShowAnimation);
	}
}

void USSModalBaseWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}
