/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSGameActivatableWidget.h"

#include "CommonUITypes.h"
#include "Editor/WidgetCompilerLog.h"
#include "Input/SSInputLocalPlayerSubsystem.h"

#define LOCTEXT_NAMESPACE "FSomndusGameModule"

USSGameActivatableWidget::USSGameActivatableWidget()
{
}

TOptional<FUIInputConfig> USSGameActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case ESSWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case ESSWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case ESSWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case ESSWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}

#if WITH_EDITOR

void USSGameActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
	IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);

	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(USSGameActivatableWidget, BP_GetDesiredFocusTarget)))
	{
		if (GetParentNativeClass(GetClass()) == USSGameActivatableWidget::StaticClass())
		{
			CompileLog.Warning(LOCTEXT("ValidateGetDesiredFocusTarget_Warning", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen."));
		}
		else
		{
			//TODO - Note for now, because we can't guarantee it isn't implemented in a native subclass of this one.
			CompileLog.Note(LOCTEXT("ValidateGetDesiredFocusTarget_Note", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.  If it was implemented in the native base class you can ignore this message."));
		}
	}
}

FString USSGameActivatableWidget::GetInputToken()
{
	return GetName();
}

void USSGameActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	if (CommonUI::IsEnhancedInputSupportEnabled())
	{
		if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
		{
			if (USSInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<USSInputLocalPlayerSubsystem>())
			{
				InputSystem->PullGlobalMappingContext(GetInputToken());
			}
		}
	}
}

void USSGameActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	if (CommonUI::IsEnhancedInputSupportEnabled())
	{
		if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
		{
			if (USSInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<USSInputLocalPlayerSubsystem>())
			{
				InputSystem->PopGlobalMappingContext(GetInputToken());
			}
		}
	}
}

#endif

#undef LOCTEXT_NAMESPACE