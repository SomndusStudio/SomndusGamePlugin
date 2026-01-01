/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UI/SSGameActivatableWidget.h"
#include "SSModalBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSModalBaseWidget : public USSGameActivatableWidget
{
	GENERATED_BODY()

	USSModalBaseWidget();

protected:
	
	virtual void NativePreConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidget> OverlayModalRoot;
};
