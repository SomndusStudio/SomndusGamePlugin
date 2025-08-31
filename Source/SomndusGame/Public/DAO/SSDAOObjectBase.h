/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSContextObject.h"
#include "SSDAOObjectBase.generated.h"

/**
 * An abstract base class derived from USSContextObject that provides a foundation for specialized DAO (Data Access Object) classes
 * within the SOMNDUSGAME module. This class contains the core initialization logic necessary for DAO objects.
 *
 * DAO objects inheriting from this class typically encapsulate data access logic and allow structured management of game-specific
 * data objects. Its functionality is often accessed and managed through USSDAOManager.
 *
 * Key Features:
 * - Inherits functionality from USSContextObject.
 * - Provides the ability to initialize objects via the `NativeInitialize` method.
 */
UCLASS(Abstract)
class SOMNDUSGAME_API USSDAOObjectBase : public USSContextObject
{
	GENERATED_BODY()

public:

	virtual void NativeInitialize();
};
