// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSEnvironmentDataAsset.h"
#include "Components/ActorComponent.h"
#include "SSEnvironmentComponent.generated.h"

/**
 * Component that handles character-to-environment interactions such as footsteps, FX, and sounds.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOMNDUSGAME_API USSEnvironmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USSEnvironmentComponent();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag FootstepFallback = SSGameplayTags::Tag_Env_Footstep_Default;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag SurfaceFallback = SSGameplayTags::Tag_Env_SurfaceType_Default;
	
	/** Reference to the environment data asset containing all environmental configurations. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USSEnvironmentDataAsset> EnvironmentDataAsset;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Cached action tag of the last valid footstep. */
	FGameplayTag CachedActionTag;

	/** Cached surface tag of the last valid footstep. */
	FGameplayTag CachedSurfaceTag;

	/** Cached surface definition pointer (not owned). */
	const FSSFootstepSurfaceDefinition* CachedFootstepSurface = nullptr;

	void InvalidateFootstepCache();
	
public:

	const FSSFootstepDefinition* TryGetFootstepDefinition(const USSFootstepDataAsset* FootstepAsset, FGameplayTag ActionTag, FGameplayTag SurfaceTag);
	const FSSFootstepSurfaceDefinition* TryGetFootstepSurfaceDefinition(const USSFootstepDataAsset* FootstepAsset, FGameplayTag ActionTag, FGameplayTag SurfaceTag);
	
	/**
	 * Executes a footstep effect based on the given action and surface tags.
	 *
	 * @param ActionTag The gameplay tag describing the footstep action (walk, run, land, etc).
	 * @param SurfaceTag The gameplay tag describing the surface type (wood, grass, etc).
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment")
	bool ExecuteFootstepSurface(FGameplayTag ActionTag, FGameplayTag SurfaceTag);

	UFUNCTION(BlueprintCallable, Category = "Environment")
	bool ExecuteFootstep(FGameplayTag ActionTag);
};
