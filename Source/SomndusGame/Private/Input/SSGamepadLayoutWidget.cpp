// ©2025 Schartier Isaac. All rights reserved.


#include "Input/SSGamepadLayoutWidget.h"

#include "InputMappingContext.h"
#include "Components/CanvasPanel.h"
#include "SSLog.h"
#include "Input/SSGamepadButtonCaptureWidget.h"

void USSGamepadLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CaptureGameButtonWidgets();
	
	if (!DefaultMappingContexts.IsEmpty())
	{
		UpdateFromMappingContexts(DefaultMappingContexts);
	}
}

void USSGamepadLayoutWidget::UpdateFromMappingContexts(const TArray<UInputMappingContext*>& InInputMappingContexts)
{
	MappingContexts = InInputMappingContexts;
	
	// Clear
	for (const auto& Pair : GamepadButtonWidgets)
	{
		auto* ButtonWidget = Pair.Value;
		ButtonWidget->ClearInputAction();
	}
	
	// Update
	for (const auto& MappingContext : MappingContexts)
	{
		for (const auto& Mapping : MappingContext->GetMappings())
		{
			auto* ButtonWidget = GetButtonWidgetFromKey(Mapping.Key);
			if (!ButtonWidget)
			{
				UE_LOG(LogSomndusGame, Warning, TEXT("[%s] No button widget found for key %s"), *GetNameSafe(this), *Mapping.Key.ToString());
				continue;
			}
			ButtonWidget->SetInputAction(Mapping.Action);
		}
	}
}

bool USSGamepadLayoutWidget::CaptureGameButtonWidgets()
{
	if (!ButtonsLayoutPanel)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("[%s] No ButtonsLayoutPanel found"), *GetNameSafe(this));
		return false;
	}

	for (auto* ChildWidget : ButtonsLayoutPanel->GetAllChildren()){

		if (USSGamepadButtonCaptureWidget* ButtonCaptureWidget = Cast<USSGamepadButtonCaptureWidget>(ChildWidget))
		{
			FKey Key = ButtonCaptureWidget->GetPrimaryAttachedKey();
			GamepadButtonWidgets.Add(Key, ButtonCaptureWidget);
		}
	}
	
	return true;
}

USSGamepadButtonCaptureWidget* USSGamepadLayoutWidget::GetButtonWidgetFromKey(const FKey& InKey)
{
	return GamepadButtonWidgets.FindRef(InKey);
}
