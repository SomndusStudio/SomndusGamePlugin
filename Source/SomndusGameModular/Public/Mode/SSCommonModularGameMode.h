// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "SSCommonModularGameMode.generated.h"

class USSPawnData;
class USSGameModeDefinition;

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API ASSCommonModularGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

protected:
	
	ASSCommonModularGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	static ASSCommonModularGameMode* Get(const UObject* WorldContextObject);
	
	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	//~End of AGameModeBase interface

	UFUNCTION(BlueprintCallable, Category = "DU|Pawn")
	const USSPawnData* GetPawnDataForController(const AController* InController) const;

	void OnGameModeLoaded(const USSGameModeDefinition* CurrentGameMode);
	bool IsGameModeLoaded() const;

	void OnMatchAssignmentGiven(FPrimaryAssetId GameModeId, const FString& GameModeIdSource);

	void HandleMatchAssignmentIfNotExpectingOne();
};
