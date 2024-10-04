/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSGameModule.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameModule : public USSContextObject
{
	GENERATED_BODY()

public:
	
	virtual void NativeInit();
};
