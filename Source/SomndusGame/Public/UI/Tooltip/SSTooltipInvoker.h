/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSTooltipWidgetBase.h"
#include "UObject/Interface.h"
#include "SSTooltipInvoker.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USSTooltipInvoker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOMNDUSGAME_API ISSTooltipInvoker
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SS|UI|Item")
	FVector2D GetTooltipOffsetPosition(UUserWidget* TooltipWidgetBase);
	
};
