﻿/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonRichTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Components/TextBlock.h"
#include "Messaging/CommonGameDialog.h"
#include "SSAlertModal.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSAlertModal : public UCommonGameDialog
{
	GENERATED_BODY()
public:
	virtual void SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback) override;
	virtual void KillDialog() override;

	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	
protected:
	virtual void CloseAlertWindow(ECommonMessagingResult Result);

private:
	
	FCommonMessagingResultDelegate OnResultCallback;

public:
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> EntryBox_Buttons;
	
private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Title;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> RichText_Description;
};
