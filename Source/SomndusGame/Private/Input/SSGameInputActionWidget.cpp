/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSGameInputActionWidget.h"

#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"
#include "Input/SSInputStaticsLibrary.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

UCommonInputSubsystem* USSGameInputActionWidget::GetInputSubsystem() const
{
	const ULocalPlayer* BindingOwner = GetOwningLocalPlayer();
	return UCommonInputSubsystem::Get(BindingOwner);
}

TSharedRef<SWidget> USSGameInputActionWidget::RebuildWidget()
{
	MyKeyBox = SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center);

	MyKeyBox->SetContent(	
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MyIcon, SImage)
			.Image(&Icon)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(KeyboardBackground, SImage)
			.Image(&KeyboardBackgroundBrush)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(FMargin(5.0f))
		[
			SAssignNew(KeyboardTextBlock, STextBlock)
			.Font(KeyboardFontStyle)
			.Text(FText::FromString(TEXT("???")))
			.ColorAndOpacity(FSlateColor(FLinearColor::Black))
		]);
	
	return MyKeyBox.ToSharedRef();
}

void USSGameInputActionWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	UpdateInputActionWidget();
	ListenToInputMethodChanged();
}

void USSGameInputActionWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	MyIcon.Reset();
	MyKeyBox.Reset();
	
	ListenToInputMethodChanged(false);
	Super::ReleaseSlateResources(bReleaseChildren);
}

void USSGameInputActionWidget::Refresh()
{
	UpdateInputActionWidget();
}

void USSGameInputActionWidget::UpdateInputActionWidget()
{
	if (GetWorld())
	{
		const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
		if (IsDesignTime() || (GetGameInstance() && ensure(CommonInputSubsystem) && CommonInputSubsystem->ShouldShowInputKeys()))
		{
			if (ShouldUpdateActionWidgetIcon())
			{
				Icon = GetIcon();

				// -- if icon not found : try generic keyboard style
				if (Icon.DrawAs == ESlateBrushDrawType::NoDrawType)
				{
					// Update dynamic keyboard
					if (UpdateKeyboardAction(CommonInputSubsystem))
					{
						return;
					}
					SetVisibility(ESlateVisibility::Collapsed);
				}
				// -- icon exist
				else if (MyIcon.IsValid())
				{
					// hide dynamic keyboard icon
					if (KeyboardBackground && KeyboardTextBlock)
					{
						KeyboardBackground->SetVisibility(EVisibility::Collapsed);
						KeyboardTextBlock->SetVisibility(EVisibility::Collapsed);
					}
					MyIcon->SetVisibility(EVisibility::Visible);
					
					MyIcon->SetImage(&Icon);

					if (GetVisibility() != ESlateVisibility::Collapsed)
					{
						// The object being passed into SetImage is the same each time so layout is never invalidated
						// Manually invalidate it here as the dimensions may have changed
						MyIcon->Invalidate(EInvalidateWidgetReason::Layout);
					}

					MyKeyBox->Invalidate(EInvalidateWidget::LayoutAndVolatility);
					SetVisibility(IsDesignTime() ? ESlateVisibility::Visible : ESlateVisibility::SelfHitTestInvisible);

					return;
				}
			}
		}

		SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool USSGameInputActionWidget::UpdateKeyboardAction(const UCommonInputSubsystem* CommonInputSubsystem)
{
	// Update dynamic keyboard
	if (KeyboardBackground && KeyboardTextBlock)
	{
		KeyDefaultText = FText::FromString(FString(""));
		if (!IsDesignTime())
		{
			if (bForceInputType)
			{
				KeyDefaultText = USSInputStaticsLibrary::GetKeyNameForInputActionType(CommonInputSubsystem, InputAction, bGamepad);
			}
			else
			{
				KeyDefaultText = USSInputStaticsLibrary::GetKeyNameForEnhancedInputAction(CommonInputSubsystem, InputAction);
			}
		}
#if WITH_EDITORONLY_DATA
		else
		{
			if (!DesignTimeKey.IsGamepadKey())
			{
				KeyDefaultText = DesignTimeKey.GetDisplayName();
			}
		}
#endif
		// stop if not keyboard key (ignore gamepad)
		if (KeyDefaultText.IsEmpty())
		{
			return false;
		}
		KeyboardBackground->SetVisibility(EVisibility::SelfHitTestInvisible);
		KeyboardTextBlock->SetVisibility(EVisibility::SelfHitTestInvisible);
		KeyboardTextBlock->SetText(KeyDefaultText);
	}
	if (MyIcon)
	{
		MyIcon->SetVisibility(EVisibility::Collapsed);
	}
	SetVisibility(IsDesignTime() ? ESlateVisibility::Visible : ESlateVisibility::SelfHitTestInvisible);
				
	return true;
}

void USSGameInputActionWidget::ListenToInputMethodChanged(bool bListen)
{
	if (UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem())
	{
		CommonInputSubsystem->OnInputMethodChangedNative.RemoveAll(this);
		if (bListen && !bForceInputType)
		{
			CommonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);
		}
	}
}

void USSGameInputActionWidget::HandleInputMethodChanged(ECommonInputType InInputType)
{
	UpdateInputActionWidget();
}

bool USSGameInputActionWidget::ShouldUpdateActionWidgetIcon() const
{
	const bool bIsInputAction = IsValid(InputAction);

#if WITH_EDITORONLY_DATA
	const bool bIsDesignPreview = IsDesignTime() && DesignTimeKey.IsValid();
#else
	const bool bIsDesignPreview = false;
#endif

	return bIsInputAction || bIsDesignPreview;
}

FSlateBrush USSGameInputActionWidget::GetIcon() const
{
	if (!IsDesignTime())
	{
		if (const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem())
		{
			// if force gamepad
			if (bForceInputType)
			{
				return USSInputStaticsLibrary::GetIconForInputActionType(CommonInputSubsystem, InputAction, bGamepad);
			}
			return USSInputStaticsLibrary::GetIconForEnhancedInputAction(CommonInputSubsystem, InputAction);
		}
	}
#if WITH_EDITORONLY_DATA
	else
	{
		if (DesignTimeKey.IsValid())
		{
			ECommonInputType Dummy;
			FName OutDefaultGamepadName;
			FCommonInputBase::GetCurrentPlatformDefaults(Dummy, OutDefaultGamepadName);

			ECommonInputType KeyInputType = ECommonInputType::MouseAndKeyboard;
			if (DesignTimeKey.IsGamepadKey())
			{
				KeyInputType = ECommonInputType::Gamepad;
			}
			else if (DesignTimeKey.IsTouch())
			{
				KeyInputType = ECommonInputType::Touch;
			}

			FSlateBrush InputBrush;
			if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(InputBrush, TArray<FKey> { DesignTimeKey }, KeyInputType, OutDefaultGamepadName))
			{
				return InputBrush;
			}
		}
	}
#endif

	return *FStyleDefaults::GetNoBrush();
}

void USSGameInputActionWidget::SetInputAction(UInputAction* InInputAction)
{
	InputAction = InInputAction;
	UpdateInputActionWidget();
}
