/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSInputActionWidget.h"

#include "CommonUITypes.h"
#include "Input/SSInputStaticsLibrary.h"

bool USSInputActionWidget::UpdateKeyboardAction(const UCommonInputSubsystem* CommonInputSubsystem)
{
	// Update dynamic keyboard
	if (KeyboardBackground && KeyboardTextBlock)
	{
		if (CommonUI::IsEnhancedInputSupportEnabled())
		{
			KeyDefaultText = FText::FromString(FString(""));
			if (!IsDesignTime())
			{
				KeyDefaultText = USSInputStaticsLibrary::GetKeyNameForEnhancedInputAction(CommonInputSubsystem, EnhancedInputAction);
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
		if (MyProgressImage)
		{
			if (IsHeldAction())
			{
				MyProgressImage->SetVisibility(EVisibility::SelfHitTestInvisible);
			}
			else
			{
				MyProgressImage->SetVisibility(EVisibility::Collapsed);
			}
		}
		if (MyIcon)
		{
			MyIcon->SetVisibility(EVisibility::Collapsed);
		}
		SetVisibility(IsDesignTime() ? ESlateVisibility::Visible : ESlateVisibility::SelfHitTestInvisible);
					
		return true;
	}
	return false;
}


void USSInputActionWidget::UpdateActionWidget()
{
	if (GetWorld())
	{
		const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
		if (IsDesignTime() || (GetGameInstance() && ensure(CommonInputSubsystem) && CommonInputSubsystem->ShouldShowInputKeys()))
		{
			if (ShouldUpdateActionWidgetIcon())
			{
				Icon = GetIcon();

				// if Icon not found
				if (Icon.DrawAs == ESlateBrushDrawType::NoDrawType)
				{
					// Update dynamic keyboard
					if (UpdateKeyboardAction(CommonInputSubsystem))
					{
						return;
					}
					SetVisibility(ESlateVisibility::Collapsed);
				}
				// Icon exist
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

					if (IsHeldAction())
					{
						MyProgressImage->SetVisibility(EVisibility::SelfHitTestInvisible);
					}
					else
					{
						MyProgressImage->SetVisibility(EVisibility::Collapsed);
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


TSharedRef<SWidget> USSInputActionWidget::RebuildWidget()
{
	if (!IsDesignTime() && ProgressDynamicMaterial == nullptr)
	{
		UMaterialInstanceDynamic* const ParentMaterialDynamic = Cast<UMaterialInstanceDynamic>(ProgressMaterialBrush.GetResourceObject());
		if (ParentMaterialDynamic == nullptr)
		{
			UMaterialInterface* ParentMaterial = Cast<UMaterialInterface>(ProgressMaterialBrush.GetResourceObject());
			if (ParentMaterial)
			{
				ProgressDynamicMaterial = UMaterialInstanceDynamic::Create(ParentMaterial, nullptr);
				ProgressMaterialBrush.SetResourceObject(ProgressDynamicMaterial);
			}
			else
			{
				ProgressDynamicMaterial = nullptr;
			}
		}
	}

	MyKeyBox = SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center);

	MyKeyBox->SetContent(	
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MyIconRim, SImage)
			.Image(&IconRimBrush)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MyProgressImage, SImage)
			.Image(&ProgressMaterialBrush)
		]
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
		]);
	
	return MyKeyBox.ToSharedRef();
}