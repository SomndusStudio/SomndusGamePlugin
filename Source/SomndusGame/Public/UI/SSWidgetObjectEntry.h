/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSWidgetObjectEntry.generated.h"


UINTERFACE()
class USSWidgetObjectEntry : public UInterface
{
	GENERATED_BODY()
};

/**
 * The ISSWidgetObjectEntry interface defines the necessary functionality for widget objects
 * that can have item objects assigned or updated. Classes implementing this interface
 * are expected to provide logic for handling the assignment and updating of item objects.
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
