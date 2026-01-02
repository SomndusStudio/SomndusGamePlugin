/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Settings/Keyboard/SSKeyInputSettingWidget.h"

#include "CommonUIExtensions.h"
#include "Input/SSInputStaticsLibrary.h"
#include "Settings/SSCommonGameUserSettings.h"
#include "Settings/Keyboard/SSKeyInputSetting.h"
#include "UI/Button/SSButtonIcon.h"
#include "Widgets/Misc/GameSettingPressAnyKey.h"
#include "Widgets/Misc/KeyAlreadyBoundWarning.h"

#define LOCTEXT_NAMESPACE "DUSettings"

void USSKeyInputSettingWidget::SetSetting(UGameSetting* InSetting)
{
	KeyInputSetting = CastChecked<USSKeyInputSetting>(InSetting);
	
	Super::SetSetting(InSetting);

	Refresh();
}

bool USSKeyInputSettingWidget::IsGamepad() const
{
	if(KeyInputSetting)
	{
		return KeyInputSetting->IsGamepad();
	}
	return false;
}

FPlayerKeyMapping USSKeyInputSettingWidget::GetPlayerKeyMapping() const
{
	return KeyInputSetting->GetKeyFromSlot(EPlayerMappableKeySlot::First);
}

int32 USSKeyInputSettingWidget::GetKeyBindSlot() const
{
	return static_cast<uint8>(GetPlayerKeyMapping().GetSlot());
}

void USSKeyInputSettingWidget::OnInputSettingChanged()
{
	OnSettingChanged();
}

void USSKeyInputSettingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_PrimaryKey->OnClicked().AddUObject(this, &ThisClass::HandleKeyClicked);

	// Bind on reset input called to force refresh himself
	// Force refresh game setting
	auto* GameUserSettings = USSCommonGameUserSettings::Get();
	GameUserSettings->OnInputNeedsUpdate.AddUniqueDynamic(this, &USSKeyInputSettingWidget::OnInputSettingChanged);
}

void USSKeyInputSettingWidget::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();

	KeyInputSetting = nullptr;
}

void USSKeyInputSettingWidget::OnSettingChanged()
{
	Refresh();
}

void USSKeyInputSettingWidget::HandleKeyClicked()
{
	UGameSettingPressAnyKey* PressAnyKeyPanel = CastChecked<UGameSettingPressAnyKey>(
	UCommonUIExtensions::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), ModalLayer, PressAnyKeyPanelClass));
	PressAnyKeyPanel->bOnlyGamepad = IsGamepad();
	PressAnyKeyPanel->OnKeySelected.AddUObject(this, &ThisClass::HandleKeySelected, PressAnyKeyPanel);
	PressAnyKeyPanel->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, PressAnyKeyPanel);
}

void USSKeyInputSettingWidget::HandleKeySelected(FKey InKey, UGameSettingPressAnyKey* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	ChangeBinding(GetKeyBindSlot(), InKey);
}

void USSKeyInputSettingWidget::HandleKeySelectionCanceled(UGameSettingPressAnyKey* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

void USSKeyInputSettingWidget::HandleKeySelectionCanceled(UKeyAlreadyBoundWarning* PressAnyKeyPanel)
{
	PressAnyKeyPanel->OnKeySelectionCanceled.RemoveAll(this);
}

void USSKeyInputSettingWidget::HandleDuplicateKeySelected(FKey InKey,
                                                          UKeyAlreadyBoundWarning* DuplicateKeyPressAnyKeyPanel) const
{
	DuplicateKeyPressAnyKeyPanel->OnKeySelected.RemoveAll(this);
	KeyInputSetting->ChangeBinding(GetKeyBindSlot(), OriginalKeyToBind);

	// Should refresh ui input cause it's a key replace
	auto* GameUserSettings = USSCommonGameUserSettings::Get();
	GameUserSettings->OnInputNeedsUpdate.Broadcast();
}

void USSKeyInputSettingWidget::ChangeBinding(int32 InKeyBindSlot, FKey InKey)
{
	OriginalKeyToBind = InKey;
	TArray<FName> ActionsForKey;
	KeyInputSetting->GetAllMappedActionsFromKey(InKeyBindSlot, InKey, ActionsForKey);
	// If detected another mapping
	if (!ActionsForKey.IsEmpty())
	{
		UKeyAlreadyBoundWarning* KeyAlreadyBoundWarning = CastChecked<UKeyAlreadyBoundWarning>(
		UCommonUIExtensions::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), ModalLayer, KeyAlreadyBoundWarningPanelClass));

		FString ActionNames;
		for (FName ActionName : ActionsForKey)
		{
			ActionNames += ActionName.ToString() += ", ";
		}

		FFormatNamedArguments Args;
		Args.Add(TEXT("InKey"), InKey.GetDisplayName());
		Args.Add(TEXT("ActionNames"), FText::FromString(ActionNames));

		KeyAlreadyBoundWarning->SetWarningText(FText::Format(LOCTEXT("Msg_KeyBinding_WarningText", "<Accent>[{InKey}]</> is already bound to <Accent>{ActionNames}</> are you sure you want to rebind it?"), Args));
		KeyAlreadyBoundWarning->SetCancelText(FText::Format(LOCTEXT("Msg_KeyBinding_CancelText", "Press escape to cancel, or press <Accent>[{InKey}]</> again to confirm rebinding."), Args));

		KeyAlreadyBoundWarning->OnKeySelected.AddUObject(this, &ThisClass::HandleDuplicateKeySelected, KeyAlreadyBoundWarning);
		
		KeyAlreadyBoundWarning->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, KeyAlreadyBoundWarning);
	}
	else
	{
		KeyInputSetting->ChangeBinding(InKeyBindSlot, InKey);
	}
}

void USSKeyInputSettingWidget::Refresh()
{
	if (ensure(KeyInputSetting))
	{
		const FPlayerKeyMapping FoundPlayerKeyMapping = GetPlayerKeyMapping();
		if (FoundPlayerKeyMapping.IsValid())
		{
			// Sync IsGamepad with buttons
			Button_PrimaryKey->SetShowIcon(IsGamepad());
			
			if (IsGamepad())
			{
				// Icon
				if (UTexture2D* Icon = USSInputStaticsLibrary::GetInputIconFromKey(this, FoundPlayerKeyMapping.GetCurrentKey()))
				{
					Button_PrimaryKey->SetShowIcon(true);
					Button_PrimaryKey->SetIcon(Icon);
					// Empty text
					Button_PrimaryKey->SetButtonText(FText::GetEmpty());
				}
			}
			else
			{
				Button_PrimaryKey->SetShowIcon(false);
				// Text
				Button_PrimaryKey->SetButtonText(FoundPlayerKeyMapping.GetCurrentKey().GetDisplayName());
			}
		}
		else
		{
			UE_LOG(LogSomndusInput, Warning, TEXT("%s : FoundPlayerKeyMapping not valid : %s"),
				*GetNameSafe(this), *KeyInputSetting->GetKeyTextFromSlot(EPlayerMappableKeySlot::First).ToString());
		}
	}
}

#undef LOCTEXT_NAMESPACE