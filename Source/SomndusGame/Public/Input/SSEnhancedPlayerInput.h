// © 2024 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "SSEnhancedPlayerInput.generated.h"

/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSEnhancedPlayerInput : public UEnhancedPlayerInput
{
	GENERATED_BODY()

public:
	const TArray<FEnhancedActionKeyMapping>& GetPublicEnhancedActionMappings() const { return GetEnhancedActionMappings(); }
};
