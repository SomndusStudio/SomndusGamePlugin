// ©2025 Schartier Isaac. All rights reserved.


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
	CachedFootstepSurface = nullptr;
}

const FSSFootstepDefinition* USSEnvironmentComponent::TryGetFootstepDefinition(const USSFootstepDataAsset* FootstepAsset, FGameplayTag ActionTag, FGameplayTag SurfaceTag)
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

const FSSFootstepSurfaceDefinition* USSEnvironmentComponent::TryGetFootstepSurfaceDefinition(const USSFootstepDataAsset* FootstepAsset, FGameplayTag ActionTag,
	FGameplayTag SurfaceTag)
{
	if (!FootstepAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("FootstepAsset is null in %s"), *GetName());
		return nullptr;
	}

	// Check cache first
	if (CachedFootstepSurface && CachedActionTag == ActionTag && CachedSurfaceTag == SurfaceTag)
	{
		return CachedFootstepSurface;
	}

	// Try exact match: Action + Surface
	const FSSFootstepSurfaceDefinition* SurfaceDef = FootstepAsset->FindFootstepSurface(ActionTag, SurfaceTag);

	if (SurfaceDef)
	{
		CachedActionTag = ActionTag;
		CachedSurfaceTag = SurfaceTag;
		CachedFootstepSurface = SurfaceDef;
		return SurfaceDef;
	}
	// Fallback 1: Action + SurfaceFallback
	if (SurfaceFallback.IsValid())
	{
		SurfaceDef = FootstepAsset->FindFootstepSurface(ActionTag, SurfaceFallback);
		if (SurfaceDef)
		{
			CachedActionTag = ActionTag;
			CachedSurfaceTag = SurfaceFallback;
			CachedFootstepSurface = SurfaceDef;
			return SurfaceDef;
		}
	}

	// Final fallback: FootstepFallback + SurfaceFallback
	if (FootstepFallback.IsValid())
	{
		SurfaceDef = FootstepAsset->FindFootstepSurface(FootstepFallback, SurfaceFallback);
		if (SurfaceDef)
		{
			CachedActionTag = FootstepFallback;
			CachedSurfaceTag = SurfaceFallback;
			CachedFootstepSurface = SurfaceDef;
			return SurfaceDef;
		}
	}

	// No match found
	UE_LOG(LogTemp, Warning, TEXT("No footstep surface definition found for Action [%s] and Surface [%s] (including fallbacks) in %s."),
		*ActionTag.ToString(),
		*SurfaceTag.ToString(),
		*GetName());

	return nullptr;
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
	
	const FSSFootstepSurfaceDefinition* FootstepSurface = TryGetFootstepSurfaceDefinition(FootstepAsset, ActionTag, SurfaceTag);
	
	if (!FootstepSurface)
	{
		UE_LOG(LogTemp, Warning, TEXT("No matching footstep surface found for Action [%s] and Surface [%s], even after fallback."),
			*ActionTag.ToString(),
			*SurfaceTag.ToString());
		return false;
	}

	// Get effect asset
	auto FootstepEffect = USSHelperStatics::TryGetAsset(FootstepSurface->FootstepEffect);
	
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

