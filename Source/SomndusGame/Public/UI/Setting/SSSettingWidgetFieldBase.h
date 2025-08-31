/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SSSettingWidgetEntry.h"
#include "SSSettingWidgetFieldBase.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSSettingWidgetFieldBase : public UCommonUserWidget, public ISSSettingWidgetEntry
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSSettingDataObject> Data;
	
	// Focus transitioning to subwidgets for the gamepad
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetPrimaryGamepadFocusWidget();

public:

	virtual void SetData_Implementation(USSSettingDataObject* DataObject) override;
};
