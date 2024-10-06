/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSButtonText.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSButtonText : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:

	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);

	void RefreshButtonText();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);
	
	UPROPERTY(EditAnywhere, Category="Button", meta=( editcondition="bOverride_ButtonText" ))
	FText ButtonText;
};
