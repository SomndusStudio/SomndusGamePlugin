// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LoadingProcessInterface.h"
#include "Components/GameStateComponent.h"
#include "Engine/StreamableManager.h"
#include "SSGameModeManagerComponent.generated.h"

namespace UE::GameFeatures { struct FResult; }

class USSGameModeDefinition;
class USSGameModeManagerComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSSGameModeLoaded, const USSGameModeDefinition*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSSGameModeGameplayReady, USSGameModeManagerComponent*);

enum class ESSGameModeLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOMNDUSGAMEMODULAR_API USSGameModeManagerComponent : public UGameStateComponent, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:

	USSGameModeManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface
	
public:
	UFUNCTION()
	void OnGameModuleReady(class USSGameSubsystem* GameSubsystem);

	void CheckGameModuleReady();
	
	// Tries to set the current experience, either a UI or gameplay one
	void SetCurrentGameMode(FPrimaryAssetId ExperienceId);

	// Ensures the delegate is called once the experience has been loaded,
	// before others are called.
	// However, if the experience has already loaded, calls the delegate immediately.
	void CallOrRegister_OnGameModeLoaded_HighPriority(FOnSSGameModeLoaded::FDelegate&& Delegate);

	// Ensures the delegate is called once the experience has been loaded
	// If the experience has already loaded, calls the delegate immediately
	void CallOrRegister_OnGameModeLoaded(FOnSSGameModeLoaded::FDelegate&& Delegate);

	// Ensures the delegate is called once the experience has been loaded
	// If the experience has already loaded, calls the delegate immediately
	void CallOrRegister_OnGameModeLoaded_LowPriority(FOnSSGameModeLoaded::FDelegate&& Delegate);

	// This returns the current experience if it is fully loaded, asserting otherwise
	// (i.e., if you called it too soon)
	const USSGameModeDefinition* GetCurrentGameModeChecked() const;

	// Returns true if the experience is fully loaded
	bool IsGameModeLoaded() const;

	UFUNCTION()
	void OnGameplayReadyDelayed();
	
	void SetGameplayReady();
	void SetGameplayReadyDelayed();
	
	// Ensures the delegate is called once the experience has been loaded
	// If the experience has already loaded, calls the delegate immediately
	void CallOrRegister_OnGameModeGameplayReady(FOnSSGameModeGameplayReady::FDelegate&& Delegate);

private:
	UFUNCTION()
	void OnRep_CurrentGameMode();
	
	void StartGameModeLoad();
	void OnExperienceLoadComplete();

	TSharedPtr<FStreamableHandle> StreamableHandle_WidgetScreen;
	
	UFUNCTION()
	void OnLoadScreenWidgetComplete();
	
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

	void StartExecuteActions();
	
private:
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentGameMode)
	TObjectPtr<const USSGameModeDefinition> CurrentGameMode;

	ESSGameModeLoadState LoadState = ESSGameModeLoadState::Unloaded;
	bool bGameplayReady = false;
	
	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	UPROPERTY()
	FTimerHandle GameplayReadyTimerHandle;
	
	/**
	 * Delegate called when the experience has finished loading just before others
	 * (e.g., subsystems that set up for regular gameplay)
	 */
	FOnSSGameModeLoaded OnGameModeLoaded_HighPriority;

	/** Delegate called when the experience has finished loading */
	FOnSSGameModeLoaded OnGameModeLoaded;

	/** Delegate called when the experience has finished loading */
	FOnSSGameModeLoaded OnGameModeLoaded_LowPriority;

	/** Delegate called when the experience has finished loading */
	FOnSSGameModeGameplayReady OnGameModeGameplayReady;
};

#define REGISTER_GAME_MODE_GAMEPLAY_READY_RAW(ContextObj, DelegateTarget, DelegateFunc) \
{ \
auto* GameState = UGameplayStatics::GetGameState(ContextObj); \
auto* GameModeManagerComponent = GameState->FindComponentByClass<USSGameModeManagerComponent>(); \
check(GameModeManagerComponent); \
GameModeManagerComponent->CallOrRegister_OnGameModeGameplayReady(FOnSSGameModeGameplayReady::FDelegate::CreateUObject(DelegateTarget, DelegateFunc)); \
}