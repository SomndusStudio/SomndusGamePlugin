// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSCommonButtonBase.h"
#include "SSActionButton.generated.h"

class UTextBlock;
class UCommonTextBlock;

/**
 * USSActionButton
 *
 * A button widget class extending USSCommonButtonBase with support for displaying an input action name.
 */
UCLASS()
class SOMNDUSGAME_API USSActionButton : public USSCommonButtonBase
{
	GENERATED_BODY()
	
protected:

	virtual void UpdateInputActionWidget() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Text Block")
	TObjectPtr<UTextBlock> Text_ActionName;
};
