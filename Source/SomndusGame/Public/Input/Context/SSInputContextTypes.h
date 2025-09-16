// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SSInputContextTypes.generated.h"

class UInputMappingContext;

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category="Input")
	int32 Priority = 0;
	
	/** If true, then this mapping context will be registered with the settings when this game feature action is registered. */
	UPROPERTY(EditAnywhere, Category="Input")
	bool bRegisterWithSettings = true;
};

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInputMappingContextTag
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere)
	TArray<FSSInputMappingContextAndPriority> InputMappingContexts;
};

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSInputTypeModeMapping
{
	GENERATED_BODY()

	/**
	 * Compatible input types
	 */
	UPROPERTY(EditAnywhere)
	TArray<ECommonInputType> InputTypes; 
	
	UPROPERTY(EditAnywhere)
	TArray<FSSInputMappingContextTag> InputMappingContextTags;

	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> DefaultActiveTags;

	/**
	 * Check if this mapping has exactly the same InputTypes as another mapping
	 */
	bool HasSameInputTypes(const FSSInputTypeModeMapping& Other) const
	{
		// Same number of types?
		if (InputTypes.Num() != Other.InputTypes.Num())
		{
			return false;
		}

		// Compare sets (ignores order, avoids duplicates issues)
		TSet<ECommonInputType> ThisSet(InputTypes);
		TSet<ECommonInputType> OtherSet(Other.InputTypes);

		return ThisSet.Difference(OtherSet).Num() == 0 && OtherSet.Difference(ThisSet).Num() == 0;
	}
};