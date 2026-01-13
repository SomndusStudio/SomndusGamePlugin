// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "Engine/DataAsset.h"
#include "SSGameModeActionSet.generated.h"

class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API USSGameModeActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	USSGameModeActionSet();
	
	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~End of UObject interface

	//~UPrimaryDataAsset interface
#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	//~End of UPrimaryDataAsset interface

public:
	// List of actions to perform as this experience is loaded/activated/deactivated/unloaded
	UPROPERTY(EditAnywhere, Instanced, Category="Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
