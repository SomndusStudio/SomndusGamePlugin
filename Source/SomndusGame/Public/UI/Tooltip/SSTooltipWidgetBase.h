/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSTooltipWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSTooltipWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateDataObject(UObject* DataObject);
};
