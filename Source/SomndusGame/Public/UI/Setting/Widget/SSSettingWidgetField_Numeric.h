/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/Button.h"
#include "UI/Setting/SSSettingWidgetFieldBase.h"
#include "UI/Setting/Data/SSSettingData_Int.h"
#include "SSSettingWidgetField_Numeric.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSSettingWidgetField_Numeric : public USSSettingWidgetFieldBase
{
	GENERATED_BODY()

	
protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSSettingData_Int> DataInt;

	virtual void NativeOnInitialized() override;
	virtual void SetData_Implementation(USSSettingDataObject* DataObject) override;

	UFUNCTION()
	void OnLeftPressed();

	UFUNCTION()
	void OnRightPressed();

	UFUNCTION()
	void RefreshDisplay(USSSettingDataObject* DataObject);

	UFUNCTION(BlueprintImplementableEvent)
	void OnValueChanged(int32 Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDefaultValueChanged(int32 DefaultValue);

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> Text_SettingValue;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> Button_Left;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> Button_Right;
};
