/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Environment/SSEnvironmentComponent.h"

#include "Helper/SSHelperStatics.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h"


// Sets default values for this component's properties
USSEnvironmentComponent::USSEnvironmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USSEnvironmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USSEnvironmentComponent::InvalidateFootstepCache()
{
	CachedActionTag = FGameplayTag::EmptyTag;
	CachedSurfaceTag = FGameplayTag::EmptyTag;
	CachedFootstepSurface = FSSFootstepSurfaceDefinition::Empty();
}

const FSSFootstepDefinition* USSEnvironmentComponent::TryGetFootstepDefinition(const USSFootstepDataAsset* FootstepAsset, const FGameplayTag& ActionTag, const FGameplayTag& SurfaceTag)
{
	const FSSFootstepDefinition* Footstep = FootstepAsset->FindFootstep(ActionTag, SurfaceTag);
	
	// First fallback: use fallback surface
	if (!Footstep && SurfaceFallback.IsValid())
	{
		Footstep = FootstepAsset->FindFootstep(ActionTag, SurfaceFallback);
	}

	// Second fallback: use fallback action
	if (!Footstep && FootstepFallback.IsValid())
	{
		Footstep = FootstepAsset->FindFootstep(FootstepFallback, SurfaceTag);
	}

	// Final fallback: both fallback tags
	if (!Footstep && FootstepFallback.IsValid() && SurfaceFallback.IsValid())
	{
		Footstep = FootstepAsset->FindFootstep(FootstepFallback, SurfaceFallback);
	}

	return Footstep;
}

bool USSEnvironmentComponent::TryGetFootstepSurfaceDefinition(const USSFootstepDataAsset* FootstepAsset, const FGameplayTag& ActionTag,
	const FGameplayTag& SurfaceTag, FSSFootstepSurfaceDefinition& FootstepSurface)
{
	if (!FootstepAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("FootstepAsset is null in %s"), *GetName());
		return false;
	}

	// Check cache first
	if (FootstepSurface.IsValid() && CachedActionTag == ActionTag && CachedSurfaceTag == SurfaceTag)
	{
		FootstepSurface = CachedFootstepSurface;
		return true;
	}

	// Try exact match: Action + Surface
	FSSFootstepSurfaceDefinition SurfaceDef;
	
	bool bValidSurface = FootstepAsset->FindFootstepSurface(ActionTag, SurfaceTag, CachedFootstepSurface);

	if (bValidSurface)
	{
		CachedActionTag = ActionTag;
		CachedSurfaceTag = SurfaceTag;
		FootstepSurface = CachedFootstepSurface;
		return true;
	}
	// Fallback 1: Action + SurfaceFallback
	if (SurfaceFallback.IsValid())
	{
		bValidSurface = FootstepAsset->FindFootstepSurface(ActionTag, SurfaceFallback, CachedFootstepSurface);
		if (bValidSurface)
		{
			CachedActionTag = ActionTag;
			CachedSurfaceTag = SurfaceFallback;
			FootstepSurface = CachedFootstepSurface;
			return true;
		}
	}

	// Final fallback: FootstepFallback + SurfaceFallback
	if (FootstepFallback.IsValid())
	{
		bValidSurface = FootstepAsset->FindFootstepSurface(FootstepFallback, SurfaceFallback, CachedFootstepSurface);
		if (bValidSurface)
		{
			CachedActionTag = FootstepFallback;
			CachedSurfaceTag = SurfaceFallback;
			FootstepSurface = CachedFootstepSurface;
			return true;
		}
	}

	// No match found
	UE_LOG(LogTemp, Warning, TEXT("No footstep surface definition found for Action [%s] and Surface [%s] (including fallbacks) in %s."),
		*ActionTag.ToString(),
		*SurfaceTag.ToString(),
		*GetName());

	return false;
}

bool USSEnvironmentComponent::ExecuteFootstepSurface(FGameplayTag ActionTag, FGameplayTag SurfaceTag)
{
	if (!EnvironmentDataAsset || EnvironmentDataAsset->FootstepDataAsset.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("EnvironmentDataAsset or FootstepDataAsset is null in %s"), *GetName());
		return false;
	}

	const USSFootstepDataAsset* FootstepAsset = USSHelperStatics::TryGetAsset(EnvironmentDataAsset->FootstepDataAsset);
	if (!FootstepAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load FootstepDataAsset in %s"), *GetName());
		return false;
	}

	FSSFootstepSurfaceDefinition FootstepSurface;
	bool bValidSurface = TryGetFootstepSurfaceDefinition(FootstepAsset, ActionTag, SurfaceTag, FootstepSurface);
	
	if (!bValidSurface)
	{
		UE_LOG(LogTemp, Warning, TEXT("No matching footstep surface found for Action [%s] and Surface [%s], even after fallback."),
			*ActionTag.ToString(),
			*SurfaceTag.ToString());
		return false;
	}

	// Get effect asset
	auto FootstepEffect = USSHelperStatics::TryGetAsset(FootstepSurface.FootstepEffect);
	
	// Play Sound
	USoundBase* Sound = USSHelperStatics::TryGetAsset(FootstepEffect->Sound);
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetOwner()->GetActorLocation(), 1.f, 1.f);
	}

	// Spawn FX
	if (!FootstepEffect->NiagaraFX.IsNull())
	{
		UNiagaraSystem* NiagaraFX = USSHelperStatics::TryGetAsset(FootstepEffect->NiagaraFX);
	
		if (NiagaraFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraFX, GetOwner()->GetActorLocation());
		}
	}
	
	return true;
}

bool USSEnvironmentComponent::ExecuteFootstep(FGameplayTag ActionTag)
{
	// TODO : Resolve surface type
	return ExecuteFootstepSurface(ActionTag, SSGameplayTags::Tag_Env_SurfaceType_Default);
}

