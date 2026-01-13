// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "GameplayTagContainer.h"
#include "ModularPlayerState.h"
#include "Pawn/SSPawnData.h"
#include "SSModularPlayerState.generated.h"

class USSGameModeDefinition;

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API ASSModularPlayerState : public AModularPlayerState
{
	GENERATED_BODY()
	
public:
	ASSModularPlayerState(const FObjectInitializer& ObjectInitializer);

	static FName NAME_LocalPlayerDataReady;

	UPROPERTY()
	TObjectPtr<const USSPawnData> PawnData;

	UPROPERTY()
	TArray<TObjectPtr<const USSPawnData>> PawnsData;

private:

	void OnGameModeLoaded(const USSGameModeDefinition* CurrentGameMode);
	
public:
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const USSPawnData* InPawnData);
	
	FGameplayTag GetPawnTagFromActor(APawn* InPawn);

public:

	//~AActor interface
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	
	//~APlayerState interface
	virtual void ClientInitialize(AController* C) override;
	//~End of APlayerState interface
	
	/* Delegate part */
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FSSPawnDataEventSignature OnPawnChange;
};
