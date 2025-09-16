﻿// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "SSInputContextTypes.h"
#include "SSPawnInputContext.h"
#include "Components/ActorComponent.h"
#include "SSInputContextComponent.generated.h"

class UInputMappingContext;
class USSPawnData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOMNDUSGAME_API USSInputContextComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	static FName NAME_InputContextReady;
	
	USSInputContextComponent();

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<USSPawnData>> LoadedPawnsData;
	
	// Adds a Pawn Input Context to the system
	UFUNCTION(BlueprintCallable, Category="Input")
	void RegisterPawnInputContext(const FGameplayTag& ContextTag, USSPawnInputContext* InputContext);

	USSPawnInputContext* GetOrCreatePawnInputContext(const FGameplayTag& ContextTag);

	bool RegisteringPawnData(const TObjectPtr<USSPawnData> PawnData);
	
	// Called when the controlled pawn changes
	void OnPawnChanged(APawn* NewPawn);

	// Switch active input context by tag
	UFUNCTION(BlueprintCallable, Category="Input")
	void ActivateContextByTag(const FGameplayTag& Tag);

	void Init(APlayerController* InPlayerController);
	
	APlayerController* GetPlayerController();
	
	void SetEIC(UEnhancedInputComponent* InEIC);

	/**
	 * Will merge/override with existing
	 * @param InInputTypeModeMapping 
	 */
	bool AddInputTypeModeMapping(const FSSInputTypeModeMapping& InInputTypeModeMapping);
	bool RemoveInputTypeModeMapping(const FSSInputTypeModeMapping& InInputTypeModeMapping);

	bool CheckInputTypeModeMapping();
	
	UEnhancedInputLocalPlayerSubsystem* EnsureEIC();
	
	bool ActiveMappingContextInfo(const FSSInputMappingContextTag& InputMappingContextTag, bool bForceBindInputAction=false);
	bool ActiveMappingContext(const FGameplayTag& Tag, bool bForceBindInputAction=false);
	
	void BindInputActions(UInputMappingContext* InputMappingContext);
	void BindInputAction(const UInputAction* InputAction, FKey Key);
	void UnbindInputActions();

protected:

	// Currently active context
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USSPawnInputContext> ActivePawnInputContext;
	
	UPROPERTY(BlueprintReadOnly)
	UEnhancedInputComponent* EIC = nullptr;

	/**
	 * know which mapping context by tag is active, useful when we need to switch modes
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> ActiveMappingContextTags;

	/**
	 * Used for caching, avoid to search from ActiveMappingContextTags 
	 */
	UPROPERTY()
	TArray<FSSInputMappingContextTag> ActiveMappingContextInfoTags;

	UPROPERTY()
	FSSInputTypeModeMapping PreviousInputTypeModeMapping;
	
	FSSInputTypeModeMapping* CurrentInputTypeModeMapping;
	
	/**
	 * Contains specifics input mapping mode for input type
	 * Example : Keyboard, Gamepad, etc
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<FSSInputTypeModeMapping> InputTypeModeMappings;
	
	UPROPERTY()
	TArray<TObjectPtr<const UInputAction>> BoundInputActions;
	
	TArray<FEnhancedInputActionEventBinding*> BoundTriggeredInputActions;
	TArray<FEnhancedInputActionEventBinding*> BoundStartedInputActions;
	TArray<FEnhancedInputActionEventBinding*> BoundCompletedInputActions;

	void OnInputActionStarted(const FInputActionInstance& InputActionInstance);
	void OnInputActionTrigger(const FInputActionInstance& InputActionInstance);
	void OnInputActionCompleted(const FInputActionInstance& InputActionInstance);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	APlayerController* PlayerController;
	
	// Pawn contexts managed by this component
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USSPawnInputContext>> PawnInputContexts;

	// Cached PlayerController
	UPROPERTY()
	APlayerController* OwnerController;

	// Currently controlled pawn
	UPROPERTY()
	TWeakObjectPtr<APawn> ControlledPawn;
};
