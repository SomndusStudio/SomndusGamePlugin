/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UObject/Interface.h"
#include "SSCommonFocusWidget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USSCommonFocusWidget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOMNDUSGAME_API ISSCommonFocusWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SS|UI|Item")
	const UWidget* GetDefaultFocusWidget();
};
