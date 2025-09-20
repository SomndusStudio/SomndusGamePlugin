// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com

#pragma once

#include "CoreMinimal.h"
#include "SSInteractionTypes.h"
#include "UObject/Interface.h"
#include "SSInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USSInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOMNDUSGAME_API ISSInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteract(const FSSInteractionContext& SourceInteractionContext);
};
