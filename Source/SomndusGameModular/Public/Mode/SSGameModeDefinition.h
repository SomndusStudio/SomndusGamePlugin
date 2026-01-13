// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Blueprint/UserWidget.h"
#include "Pawn/SSPawnData.h"
#include "SSGameModeDefinition.generated.h"

class USSGameModeActionSet;
class UCheatManagerExtension;
class UGameFeatureAction;

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API USSGameModeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
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

	/** The loading screen widget to show when loading into (or back out of) a given experience */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=LoadingScreen)
	TSoftClassPtr<UUserWidget> LoadingScreenWidget;
	
	UPROPERTY(EditDefaultsOnly, Category=Preload)
	TObjectPtr<UPrimaryDataAsset> ModeDataAsset;
	
	/** The default pawn class to spawn for players */
	//@TODO: Make soft?
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TSoftObjectPtr<const USSPawnData> DefaultPawnData;
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	FGameplayTag DefaultPawnTag;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<const USSPawnData>> PawnsData;

	TObjectPtr<const USSPawnData> GetDefaultPawnData() const;
	
	// List of actions to perform as this experience is loaded/activated/deactivated/unloaded
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	// List of additional action sets to compose into this experience
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<USSGameModeActionSet>> ActionSets;
};
