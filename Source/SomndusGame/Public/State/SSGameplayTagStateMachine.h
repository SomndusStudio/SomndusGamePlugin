/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SSContextObject.h"
#include "SSGameplayTagStateMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSSOnGameplayStateChanged, FGameplayTag, PreviousState, FGameplayTag, NewState);


/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameplayTagStateMachine : public USSContextObject
{
	GENERATED_BODY()
	
public:
	USSGameplayTagStateMachine();

	// État actuel
	UPROPERTY(BlueprintReadOnly, Category = "State")
	FGameplayTag CurrentState;

	// États valides dans la state machine (optionnel selon logique)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FGameplayTagContainer AllowedStates;

	// Transitions valides (de -> vers)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TMap<FGameplayTag, FGameplayTagContainer> ValidTransitions;

	// Event lorsqu'on change d'état
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FSSOnGameplayStateChanged OnStateChanged;

	// Tente de changer d'état
	UFUNCTION(BlueprintCallable, Category = "State")
	bool TrySetState(FGameplayTag NewState);

	// Forcer un changement (ignorant les transitions)
	UFUNCTION(BlueprintCallable, Category = "State")
	void ForceSetState(FGameplayTag NewState);

	// Vérifie si une transition est valide
	UFUNCTION(BlueprintPure, Category = "State")
	bool CanTransitionTo(FGameplayTag NewState) const;

	UFUNCTION(BlueprintPure, Category = "State")
	FGameplayTag GetCurrentState() const;
	
	UFUNCTION(BlueprintPure, Category = "State")
	bool HasState(FGameplayTag InState);
};