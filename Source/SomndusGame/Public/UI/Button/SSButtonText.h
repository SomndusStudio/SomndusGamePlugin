/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSCommonButton.h"
#include "SSButtonText.generated.h"

/**
 * USSButtonText is a subclass of UCommonButtonBase that provides functionality for managing
 * and updating button text. This class allows integration with Unreal Engine's UI system
 * and provides flexible options for customizing button behavior.
 */
UCLASS()
class SOMNDUSGAME_API USSButtonText : public USSCommonButton
{
	GENERATED_BODY()
	
public:

	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);

	void RefreshButtonText();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);
	
	UPROPERTY(EditAnywhere, Category="Button")
	FText ButtonText;
};
