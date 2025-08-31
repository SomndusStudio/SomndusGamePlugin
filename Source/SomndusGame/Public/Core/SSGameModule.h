/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSGameModule.generated.h"

/**
 * @class USSGameModule
 * Base class for game modules, derived from USSContextObject.
 *
 * USSGameModule serves as a foundational class for implementing game modules in the framework.
 * It is designed to be extended and initialized via the NativeInit method.
 * The modules are managed and accessed through the USSGameSubsystem.
 *
 * @note This class is intended for use within the framework and can be expanded to suit specific game systems.
 */
UCLASS()
class SOMNDUSGAME_API USSGameModule : public USSContextObject
{
	GENERATED_BODY()

public:
	
	virtual void NativeInit();
};
