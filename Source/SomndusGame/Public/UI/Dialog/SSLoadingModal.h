/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonRichTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Components/TextBlock.h"
#include "Messaging/CommonGameDialog.h"
#include "SSLoadingModal.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSLoadingModal : public UCommonGameDialog
{
	GENERATED_BODY()
	
public:
	virtual void SetupDialog(UCommonGameDialogDescriptor* Descriptor, FCommonMessagingResultDelegate ResultCallback) override;
	virtual void KillDialog() override;

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	
	virtual void CloseLoadingWindow(ECommonMessagingResult Result);

private:
	
	FCommonMessagingResultDelegate OnResultCallback;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Title;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> RichText_Description;
};