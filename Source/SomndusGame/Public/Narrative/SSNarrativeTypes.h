// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "SSNarrativeTypes.generated.h"

class USSNarrativeNode;

namespace SSGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SS_CONTEXT_GLOBAL);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SS_CONTEXT_FROM_ACTOR);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SS_CONTEXT_PROFILE);
};

USTRUCT(BlueprintType)
struct FSSNarrativeProfile {

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ProfileId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Flags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags;
	
};


USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSNarrativeSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGameplayTagContainer ActiveNarrativeTags;
	
	/** Global flags (non liés à un profil) */
	UPROPERTY()
	TArray<FName> GlobalFlags;

	UPROPERTY()
	FGameplayTagContainer GlobalTags;

	/** All narrative profiles */
	UPROPERTY()
	TArray<FSSNarrativeProfile> SavedProfiles;
};

USTRUCT(BlueprintType)
struct FSSContextBinding
{
	GENERATED_BODY()

	/** Declares the type of context this graph expects (ex: Context.Global, Context.Profile.XYZ) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BindingTag;

	/** Optional parameter: profile ID, actor name, etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ContextQualifier;
};

USTRUCT(BlueprintType)
struct FSSNarrativeNodeLink
{
	GENERATED_BODY()

	/** Label of the link (optional) – used for choices, conditions, etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LinkLabel;

	/** Destination node */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USSNarrativeNode* TargetNode;
};