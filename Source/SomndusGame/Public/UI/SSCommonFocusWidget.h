/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSCommonFocusWidget.generated.h"

class UWidget;

UINTERFACE()
class USSCommonFocusWidget : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for widgets that can specify a default focusable sub-widget.
 * This is useful for widgets that have complex compositions and require
 * a particular child widget to receive focus by default.
 */
class SOMNDUSGAME_API ISSCommonFocusWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SS|UI|Item")
	const UWidget* GetDefaultFocusWidget();
};
