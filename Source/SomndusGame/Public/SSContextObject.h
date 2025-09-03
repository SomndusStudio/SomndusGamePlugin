/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/Level.h"
#include "SSContextObject.generated.h"

/**
 * USSContextObject
 *
 * A lightweight UObject subclass providing convenient access to engine-level context such as
 * the current world, level, and game instance.
 *
 * Typically used as a base for Blueprint Function Library-compatible objects or other utility-based
 * gameplay logic that needs safe access to engine systems, even outside of actors.
 */
UCLASS(Blueprintable, BlueprintType)
class SOMNDUSGAME_API USSContextObject : public UObject
{
	GENERATED_BODY()
	virtual void PostLoad() override;

	/**
	 * Returns the level this object belongs to.
	 * Useful for context-aware logic outside of actor scope.
	 */
	class ULevel* GetLevel() const;

public:
	// Allows the Object to use BP_Functions
	virtual class UWorld* GetWorld() const override;
	
	/**
	 * Returns the current game instance.
	 * Uses gameplay statics to fetch the active game context.
	 */
	UFUNCTION(BlueprintPure)
	class UGameInstance* GetGameInstance() const;
};
