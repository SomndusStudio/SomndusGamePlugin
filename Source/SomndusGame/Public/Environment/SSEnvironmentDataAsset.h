// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSEnvironmentTypes.h"
#include "Engine/DataAsset.h"
#include "SSEnvironmentDataAsset.generated.h"

/**
 * Defines a reusable footstep effect (sound + FX) used across multiple footstep definitions.
 */
UCLASS(BlueprintType)
class SOMNDUSGAME_API USSFootstepEffect : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Sound asset to play when the footstep is triggered. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	TSoftObjectPtr<USoundBase> Sound;

	/** Niagara FX to spawn when the footstep is triggered. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	TSoftObjectPtr<UNiagaraSystem> NiagaraFX;

	/** Volume multiplier for the footstep sound. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	float VolumeMultiplier = 1.0f;

	/** Pitch multiplier for the footstep sound. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	float PitchMultiplier = 1.0f;
};

/**
 * Data asset containing the full list of footstep definitions.
 * Each definition specifies how footsteps behave based on surface and action tags.
 */
UCLASS(BlueprintType)
class SOMNDUSGAME_API USSFootstepDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	/** List of all footstep definitions available in this data asset. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
	TArray<FSSFootstepDefinition> FootstepDefinitions;

	/**
	 * Finds a matching footstep definition based on action and surface tags.
	 * 
	 * @param Action Tag describing the footstep action (e.g., walk, run, land).
	 * @param Surface Tag describing the surface type (e.g., grass, wood).
	 * @return Pointer to the matching footstep definition, or nullptr if not found.
	 */
	const FSSFootstepDefinition* FindFootstep(FGameplayTag Action, FGameplayTag Surface) const
	{
		for (const FSSFootstepDefinition& Def : FootstepDefinitions)
		{
			if (Def.Matches(Action, Surface))
			{
				return &Def;
			}
		}
		return nullptr;
	}

	const FSSFootstepSurfaceDefinition* FindFootstepSurface(FGameplayTag ActionTag, FGameplayTag SurfaceTag) const
	{
		for (const FSSFootstepDefinition& Def : FootstepDefinitions)
		{
			if (Def.ActionTag == ActionTag)
			{
				if (const FSSFootstepSurfaceDefinition* SurfaceDef = Def.FindSurface(SurfaceTag))
				{
					return SurfaceDef;
				}
			}
		}
		return nullptr;
	}
};

/**
 * Environment-level data asset containing references to various environmental systems or data,
 * such as footsteps, weather, or surface interactions.
 */
UCLASS()
class SOMNDUSGAME_API USSEnvironmentDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Soft reference to the footstep data asset used by the environment system. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USSFootstepDataAsset> FootstepDataAsset;
};
