// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UObject/Object.h"
#include "SSPawnData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSSPawnDataEventSignature, class APawn*, Pawn, FGameplayTag, PawnType);

/**
 * 
 */
UCLASS(BlueprintType)
class SOMNDUSGAME_API USSPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// Class to instantiate for this pawn (should usually derive from ALyraPawn or ALyraCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DU|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DU|Pawn")
	FGameplayTag PawnTypeTag;
};
