// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "AnalogSlider.h"
#include "CommonTextBlock.h"
#include "UI/Setting/SSSettingWidgetFieldBase.h"
#include "SSSettingWidgetField_Scalar.generated.h"

class USSSettingData_Float;
/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSSettingWidgetField_Scalar : public USSSettingWidgetFieldBase
{
	GENERATED_BODY()

protected:
	bool bSuspendChangeUpdates = false;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSSettingData_Float> DataFloat;
	
	void Refresh();
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleSliderValueChanged(float Value);
	UFUNCTION()
	void HandleSliderCaptureEnded();

	UFUNCTION(BlueprintImplementableEvent)
	void OnValueChanged(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDefaultValueChanged(float DefaultValue);

	virtual void SetData_Implementation(USSSettingDataObject* DataObject) override;
	
private:	// Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UPanelWidget> Panel_Value;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UAnalogSlider> Slider_SettingValue;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> Text_SettingValue;
};
