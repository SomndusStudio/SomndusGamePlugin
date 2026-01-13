/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "SSEnvironmentTypes.generated.h"

class USSFootstepEffect;
class USoundBase;
class UNiagaraSystem;

namespace SSGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Env_SurfaceType_Default);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Env_Footstep_Default);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Env_Footstep_Left);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Env_Footstep_Right);
}

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSFootstepSurfaceDefinition
{
	GENERATED_BODY()

	/** Surface tag (e.g., Grass, Wood, Concrete). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	FGameplayTag SurfaceTag;

	/** Reference to the footstep effect asset (sound + FX). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	TSoftObjectPtr<USSFootstepEffect> FootstepEffect;

	/** 
	 * Checks whether this definition matches a given action and surface.
	 */
	bool Matches(FGameplayTag InSurface) const
	{
		return SurfaceTag == InSurface;
	}

	bool IsValid() const
	{
		return SurfaceTag.IsValid() && !FootstepEffect.IsNull();
	}

	static FSSFootstepSurfaceDefinition Empty()
	{
		return FSSFootstepSurfaceDefinition();
	}
};

/**
 * Defines a single footstep configuration based on surface and action tags.
 * Points to a shared effect asset to avoid redundancy.
 */
USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSFootstepDefinition
{
	GENERATED_BODY()

	/** Footstep action tag (e.g., Walk, Run, Land). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	FGameplayTag ActionTag;

	/** Surface (e.g., Grass, Wood, Concrete). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	TArray<FSSFootstepSurfaceDefinition> SurfaceDefinitions;

	/**
	 * Finds the matching surface definition for a given surface tag.
	 *
	 * @param InSurface The surface tag to match.
	 * @return Pointer to the matching surface definition, or nullptr if not found.
	 */
	const FSSFootstepSurfaceDefinition* FindSurface(FGameplayTag InSurface) const
	{
		for (const FSSFootstepSurfaceDefinition& SurfaceDef : SurfaceDefinitions)
		{
			if (SurfaceDef.Matches(InSurface))
			{
				return &SurfaceDef;
			}
		}
		return nullptr;
	}
	
	/** 
	 * Checks whether this definition matches a given action and surface.
	 */
	bool Matches(FGameplayTag InAction, FGameplayTag InSurface) const
	{
		if (ActionTag != InAction)
		{
			return false;
		}

		for (const FSSFootstepSurfaceDefinition& SurfaceDef : SurfaceDefinitions)
		{
			if (SurfaceDef.Matches(InSurface))
			{
				return true;
			}
		}

		return false;
	}
};