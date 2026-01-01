/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "SSEnhancedPlayerInput.generated.h"

/**
 * @class USSEnhancedPlayerInput
 * A subclass of UEnhancedPlayerInput that provides access to enhanced action key mappings.
 *
 * This class extends UEnhancedPlayerInput, adding functionality to expose enhanced action
 * key mappings publicly for use in various subsystems or gameplay systems. The purpose of this
 * class is to allow external systems to retrieve the action key mappings efficiently.
 */
UCLASS()
class SOMNDUSGAME_API USSEnhancedPlayerInput : public UEnhancedPlayerInput
{
	GENERATED_BODY()

public:
	const TArray<FEnhancedActionKeyMapping>& GetPublicEnhancedActionMappings() const { return GetEnhancedActionMappings(); }
};
