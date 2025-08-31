/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSTooltipWidgetBase.generated.h"

/**
 * USSTooltipWidgetBase is the base class for tooltip widgets used in the SOMNDUSGAME.
 * It provides a method for updating the widget with a data object that contains the information the tooltip should display.
 */
UCLASS()
class SOMNDUSGAME_API USSTooltipWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateDataObject(UObject* DataObject);
};
