/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSSettingWidgetEntry.generated.h"

class USSSettingDataObject;


UINTERFACE()
class USSSettingWidgetEntry : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOMNDUSGAME_API ISSSettingWidgetEntry
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void SetData(USSSettingDataObject* DataObject);
};
