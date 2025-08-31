// ©2025 Schartier Isaac. All rights reserved.


#include "Input/SSGamepadButtonCaptureWidget.h"

#include "InputAction.h"
#include "SSLog.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Input/SSInputStaticsLibrary.h"

void USSGamepadButtonCaptureWidget::Refresh_Implementation()
{
	NativeRefreshSlot();
	NativeRefreshExtraSlot();
}

void USSGamepadButtonCaptureWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FKey Key = AttachedKeys[0];
	UTexture2D* IconTexture = USSInputStaticsLibrary::GetGamepadInputIconFromKey(this, Key, GamepadName);
	if (IconTexture)
	{
		if (InputIconWidget)
		{
			InputIconWidget->SetBrushFromTexture(IconTexture);
		}
	}
	else{
		UE_LOG(LogSomndusGame, Warning, TEXT("[%s] No icon found for key %s"), *GetNameSafe(this), *Key.ToString());
	}
}

void USSGamepadButtonCaptureWidget::NativeRefreshSlot()
{
	if (!NameSlotWidget)
	{
		return;
	}

	// Mono text
	if (IsValid(InputAction))
	{
		NameSlotWidget->SetText(InputAction->ActionDescription);
	}
	else
	{
		NameSlotWidget->SetText(FText::FromString("???"));
	}
}

void USSGamepadButtonCaptureWidget::NativeRefreshExtraSlot()
{
	if (!ExtraNameSlotWidget)
	{
		return;
	}
	FString OutputExtra = "";
	// Extra
	for (const auto& ExtraInputAction : ExtraInputActions)
	{
		if (ExtraInputAction)
		{
			OutputExtra += " / " + ExtraInputAction->ActionDescription.ToString();
		}
	}
	ExtraNameSlotWidget->SetText(FText::FromString(OutputExtra));
}

void USSGamepadButtonCaptureWidget::SetInputAction(const UInputAction* InInputAction)
{
	if (IsValid(InputAction))
	{
		ExtraInputActions.AddUnique(InInputAction);
	}
	else
	{
		InputAction = InInputAction;
	}

	Refresh();
}

FKey USSGamepadButtonCaptureWidget::GetPrimaryAttachedKey() const
{
	return AttachedKeys[0];
}
