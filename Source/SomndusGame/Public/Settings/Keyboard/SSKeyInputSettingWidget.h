// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/SSInputLocalPlayerSubsystem.h"
#include "UI/SSCommonUITypes.h"
#include "Widgets/GameSettingListEntry.h"
#include "SSKeyInputSettingWidget.generated.h"

class USSKeyInputSetting;
class USSButtonIcon;
class UGameSettingPressAnyKey;
class UKeyAlreadyBoundWarning;

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSKeyInputSettingWidget : public UGameSettingListEntry_Setting
{
	GENERATED_BODY()

public:
	
	virtual void SetSetting(UGameSetting* InSetting) override;

	bool IsGamepad() const;

	FPlayerKeyMapping GetPlayerKeyMapping() const;
	
	int32 GetKeyBindSlot() const;
	
protected:
	
	UFUNCTION()
	void OnInputSettingChanged();
	
	virtual void NativeOnInitialized() override;
	virtual void NativeOnEntryReleased() override;
	virtual void OnSettingChanged() override;
	
	void HandleKeyClicked();

	void HandleKeySelected(FKey InKey, UGameSettingPressAnyKey* PressAnyKeyPanel);
	
	void HandleDuplicateKeySelected(FKey InKey, UKeyAlreadyBoundWarning* DuplicateKeyPressAnyKeyPanel) const;

	void HandleKeySelectionCanceled(UGameSettingPressAnyKey* PressAnyKeyPanel);
	void HandleKeySelectionCanceled(UKeyAlreadyBoundWarning* PressAnyKeyPanel);
	
	void ChangeBinding(int32 BindSlot, FKey InKey);

	void Refresh();
	
protected:

	FGameplayTag ModalLayer = SSGameplayTags::TAG_SS_LAYER_MODAL;
	
	UPROPERTY(Transient)
	FKey OriginalKeyToBind = EKeys::Invalid;
	
	UPROPERTY()
	TObjectPtr<USSKeyInputSetting> KeyInputSetting;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSettingPressAnyKey> PressAnyKeyPanelClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UKeyAlreadyBoundWarning> KeyAlreadyBoundWarningPanelClass;

private:

	// Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<USSButtonIcon> Button_PrimaryKey;
};
