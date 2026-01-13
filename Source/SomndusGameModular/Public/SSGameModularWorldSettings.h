// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"

#include "SSGameModularWorldSettings.generated.h"

class USSGameModeDefinition;

/**
 * The default world settings object, used primarily to set the default gameplay experience to use when playing on this map
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API ASSGameModularWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif
	
public:
	// Returns the default experience to use when a server opens this map if it is not overridden by the user-facing experience
	FPrimaryAssetId GetDefaultGameplayMode() const;

protected:
	// The default experience to use when a server opens this map if it is not overridden by the user-facing experience
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<USSGameModeDefinition> DefaultGameplayMode;

public:

#if WITH_EDITORONLY_DATA
	// Is this level part of a front-end or other standalone experience?
	// When set, the net mode will be forced to Standalone when you hit Play in the editor
	UPROPERTY(EditDefaultsOnly, Category=PIE)
	bool ForceStandaloneNetMode = false;
#endif
};
