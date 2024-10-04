/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSWidgetObjectEntry.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USSWidgetObjectEntry : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOMNDUSGAME_API ISSWidgetObjectEntry
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void NativeOnItemObjectSet(UObject* InItemObject);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SS|UI|Item")
	void OnItemObjectSet(UObject* InItemObject);
};
