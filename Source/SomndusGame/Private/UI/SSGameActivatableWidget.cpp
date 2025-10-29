/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "UI/SSGameActivatableWidget.h"

#include "CommonUITypes.h"
#include "TimerManager.h"
#include "Editor/WidgetCompilerLog.h"
#include "Input/CommonUIInputTypes.h"
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

#endif

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

	if (bUseAutoShowAnimation)
	{
		if (ShowAnimation)
		{
			PlayAnimationForward(ShowAnimation);
		}
	}
}

void USSGameActivatableWidget::OnDelayDeactivatedFinished()
{
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

void USSGameActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	if (GetWorld())
	{
		FTimerHandle TimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USSGameActivatableWidget::OnDelayDeactivatedFinished, 0.1f, false);
	}
}

void USSGameActivatableWidget::NativeDestruct()
{
	UnregisterAllBindings();
	
	OnDelayDeactivatedFinished();

	Super::NativeDestruct();
}

void USSGameActivatableWidget::RegisterBinding(FDataTableRowHandle InputAction, const FSSInputActionExecutedDelegate& Callback, FUIActionBindingHandle& BindingHandle)
{
	FBindUIActionArgs BindArgs(InputAction, FSimpleDelegate::CreateLambda([InputAction, Callback]()
	{
		Callback.ExecuteIfBound(InputAction.RowName);
	}));
	BindArgs.bDisplayInActionBar = true;
	
	BindingHandle = RegisterUIActionBinding(BindArgs);
	BindingHandles.Add(BindingHandle);
}

void USSGameActivatableWidget::UnregisterBinding(FUIActionBindingHandle BindingHandle)
{
	if (BindingHandle.IsValid())
	{
		BindingHandle.Unregister();
		BindingHandles.Remove(BindingHandle);
	}
}

void USSGameActivatableWidget::UnregisterAllBindings()
{
	for (FUIActionBindingHandle& Handle : BindingHandles)
	{
		Handle.Unregister();
	}
	BindingHandles.Empty();
}

#undef LOCTEXT_NAMESPACE
