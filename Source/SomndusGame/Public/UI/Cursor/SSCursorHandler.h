﻿/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UI/SSCommonUITypes.h"
#include "UObject/Interface.h"
#include "SSCursorHandler.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USSCursorHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOMNDUSGAME_API ISSCursorHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SS|UI|Item")
	FSSCursorRequestInfo GetCursorRequestInfo();
	
};
