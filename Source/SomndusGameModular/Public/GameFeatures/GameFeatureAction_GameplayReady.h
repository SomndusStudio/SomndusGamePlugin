// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameplayTagContainer.h"
#include "GameFeatureAction_GameplayReady.generated.h"

struct FWorldContext;
struct FComponentRequestHandle;

/**
 * @class UGameFeatureAction_GameplayReady
 * @brief Represents an action triggered when the gameplay of a game feature is ready.
 *
 * This class is a part of the game feature action system and is executed when a particular
 * game feature is fully initialized and its gameplay state is ready to be used. It provides
 * functionality to extend or modify gameplay behavior dynamically based on feature readiness.
 *
 * This class is intended to be extended or instantiated to implement specific functionality
 * that depends on a game feature reaching a playable state.
 *
 * @note This class should have any necessary dependencies injected or set up before being
 * executed to ensure proper behavior.
 *
 * Responsibilities of this class typically include:
 * - Listening for or being triggered by the game feature's readiness.
 * - Executing predefined actions upon feature readiness.
 * - Optionally modifying gameplay systems or states when activated.
 */
UCLASS(meta = (DisplayName = "Gameplay Ready"))
class SOMNDUSGAMEMODULAR_API UGameFeatureAction_GameplayReady : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:

	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ End UObject interface

private:

	UPROPERTY(EditAnywhere)
	TArray<FName> TagTokens;
	
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<FName> TagTokens;
		bool bGameplayReady = false;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

protected:
	
	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	
	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface
	
	void Reset(FPerContextData& ActiveData);
	
	bool CheckGameplayIsReady(AActor* Actor, FPerContextData& ActiveData);

	void NotifyGameplayIsReady(AActor* Actor);
};
