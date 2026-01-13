// ©2025 Schartier Isaac. All rights reserved.


#include "Mode/SSGameModeManagerComponent.h"


#include "Engine/AssetManager.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystemSettings.h"
#include "SSLog.h"
#include "TimerManager.h"
#include "Core/SSGameSubsystem.h"
#include "Engine/GameInstance.h"
#include "Helper/SSHelperStatics.h"
#include "Mode/SSGameModeActionSet.h"
#include "Mode/SSGameModeDefinition.h"
#include "Net/UnrealNetwork.h"
#include "UI/SSLoadingScreenSubsystem.h"


USSGameModeManagerComponent::USSGameModeManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void USSGameModeManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentGameMode);
}

bool USSGameModeManagerComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (LoadState != ESSGameModeLoadState::Loaded)
	{
		OutReason = TEXT("Game Mode still loading");
		return true;
	}
	if (!bGameplayReady)
	{
		OutReason = TEXT("Game Mode not ready");
		return true;
	}
	return false;
}

void USSGameModeManagerComponent::OnGameModuleReady(USSGameSubsystem* GameSubsystem)
{
	StartGameModeLoad();
}

void USSGameModeManagerComponent::CheckGameModuleReady()
{
	auto* GameSubsystem = USSGameSubsystem::Get(this);
	OnGameModuleReady(GameSubsystem);
	/*
	if (GameSubsystem->IsGameModuleReady())
	{
		OnGameModuleReady(GameSubsystem);
	}
	else
	{
		GameSubsystem->OnGameModuleReady.AddUniqueDynamic(this, &ThisClass::OnGameModuleReady);
	}
	*/
}

void USSGameModeManagerComponent::OnRep_CurrentGameMode()
{
	CheckGameModuleReady();
}

void USSGameModeManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//@TODO: Ensure proper handling of a partially-loaded state too
	if (LoadState == ESSGameModeLoadState::Loaded)
	{
		LoadState = ESSGameModeLoadState::Deactivating;

		// Make sure we won't complete the transition prematurely if someone registers as a pauser but fires immediately
		NumExpectedPausers = INDEX_NONE;
		NumObservedPausers = 0;

		// Deactivate and unload the actions
		FGameFeatureDeactivatingContext Context(
			TEXT(""), [this](FStringView) { this->OnActionDeactivationCompleted(); });

		const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (ExistingWorldContext)
		{
			Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
		}

		auto DeactivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (UGameFeatureAction* Action : ActionList)
			{
				if (Action)
				{
					Action->OnGameFeatureDeactivating(Context);
					Action->OnGameFeatureUnregistering();
				}
			}
		};

		DeactivateListOfActions(CurrentGameMode->Actions);
		for (const TObjectPtr<USSGameModeActionSet>& ActionSet : CurrentGameMode->ActionSets)
		{
			if (ActionSet != nullptr)
			{
				DeactivateListOfActions(ActionSet->Actions);
			}
		}

		if (NumExpectedPausers > 0)
		{
			UE_LOG(LogSomndusGame, Error,
			       TEXT("Actions that have asynchronous deactivation aren't fully supported yet in Lyra experiences"));
		}

		if (NumExpectedPausers == NumObservedPausers)
		{
			OnAllActionsDeactivated();
		}
	}
}

void USSGameModeManagerComponent::StartGameModeLoad()
{
	check(CurrentGameMode != nullptr);
	check(LoadState == ESSGameModeLoadState::Unloaded);

	UE_LOG(LogSomndusGame, Log, TEXT("EXPERIENCE: StartExperienceLoad(CurrentExperience = %s)"),
	       *CurrentGameMode->GetPrimaryAssetId().ToString());

	LoadState = ESSGameModeLoadState::Loading;
	

	// LOAD ASSET BUNDLE SYSTEM
	UAssetManager& AssetManager = UAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;

	BundleAssetList.Add(CurrentGameMode->GetPrimaryAssetId());
	for (const TObjectPtr<USSGameModeActionSet>& ActionSet : CurrentGameMode->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
		}
	}

	// Load explicit ModeDataAsset
	if (CurrentGameMode->ModeDataAsset)
	{
		FSoftObjectPath ModeDataAssetSoftPath = FSoftObjectPath(CurrentGameMode->ModeDataAsset);
		FName PrimaryModeAssetName = FName(ModeDataAssetSoftPath.GetAssetName());
		FName PrimaryModeAssetPath = FName(CurrentGameMode->ModeDataAsset->GetPathName());

		const FPrimaryAssetId AssetId = FPrimaryAssetId(PrimaryModeAssetName, PrimaryModeAssetPath);

		UE_LOG(LogSomndusGame, Log, TEXT("Generate Dynamic Mode Asset to preload as : %s - %s"),
		       *PrimaryModeAssetName.ToString(), *PrimaryModeAssetPath.ToString());

		// Register a dynamic Asset
		FAssetBundleData ModeAssetBundleData;
		AssetManager.AddDynamicAsset(AssetId, FSoftObjectPath(CurrentGameMode->ModeDataAsset), ModeAssetBundleData);

		BundleAssetList.Add(AssetId);
	}

	// Load assets associated with the experience
	TArray<FName> BundlesToLoad;

	//@TODO: Centralize this client/server stuff into the LyraAssetManager
	const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
	const bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
	const bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
	if (bLoadClient)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
	}
	if (bLoadServer)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	}
	
	TSharedPtr<FStreamableHandle> BundleLoadHandle = nullptr;
	if (BundleAssetList.Num() > 0)
	{
		BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(
			BundleAssetList.Array(), {BundlesToLoad}, {}, false, FStreamableDelegate(),
			FStreamableManager::AsyncLoadHighPriority);
	}

	// For now not combine or bundle and raw, only bundle
	TSharedPtr<FStreamableHandle> Handle = nullptr;
	Handle = BundleLoadHandle;

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(
		this, &ThisClass::OnExperienceLoadComplete);
	// If loading already completed/loaded in previous level
	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// Assets were already loaded, call the delegate now
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);

		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
		{
			OnAssetsLoadedDelegate.ExecuteIfBound();
		}));
	}

	// This set of assets gets preloaded, but we don't block the start of the experience based on it
	TSet<FPrimaryAssetId> PreloadAssetList;
	//@TODO: Determine assets to preload (but not blocking-ly)
	if (PreloadAssetList.Num() > 0)
	{
		AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
	}
}

void USSGameModeManagerComponent::OnExperienceLoadComplete()
{
	check(LoadState == ESSGameModeLoadState::Loading);
	check(CurrentGameMode != nullptr);

	UE_LOG(LogSomndusGame, Log, TEXT("EXPERIENCE: OnExperienceLoadComplete(CurrentExperience = %s)"),
	       *CurrentGameMode->GetPrimaryAssetId().ToString());

	OnExperienceFullLoadCompleted();
}

void USSGameModeManagerComponent::OnLoadScreenWidgetComplete()
{
#if !UE_SERVER
	auto* LoadingScreenSubsystem = GetGameInstance<UGameInstance>()->GetSubsystem<USSLoadingScreenSubsystem>();
	check(LoadingScreenSubsystem);

	auto LoadedLoadingScreenWidget = USSHelperStatics::TryGetClass(CurrentGameMode->LoadingScreenWidget);
	LoadingScreenSubsystem->SetLoadingScreenContentWidget(LoadedLoadingScreenWidget);
	
	StartExecuteActions();
#endif
}

void USSGameModeManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ESSGameModeLoadState::Loaded);

	bool bShouldExecuteActions = true;

#if !UE_SERVER
	// Load async properly
	if (!CurrentGameMode->LoadingScreenWidget.IsNull())
	{
		// In client side, shouldn't execute actions yet cause need to load loading screen asset before
		bShouldExecuteActions = false;

		FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();

		// Load loading screen widget class
		StreamableHandle_WidgetScreen = Streamable.RequestAsyncLoad(
			CurrentGameMode->LoadingScreenWidget.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &USSGameModeManagerComponent::OnLoadScreenWidgetComplete)
		);
	}
#endif

	if (bShouldExecuteActions)
	{
		StartExecuteActions();
	}
}

void USSGameModeManagerComponent::StartExecuteActions()
{
	// Insert a random delay for testing (if configured)
	if (LoadState != ESSGameModeLoadState::LoadingChaosTestingDelay)
	{
		const float DelaySecs = 0.0f;
		if (DelaySecs > 0.0f)
		{
			FTimerHandle DummyHandle;

			LoadState = ESSGameModeLoadState::LoadingChaosTestingDelay;
			GetWorld()->GetTimerManager().SetTimer(DummyHandle, this, &ThisClass::OnExperienceFullLoadCompleted,
			                                       DelaySecs, /*bLooping=*/ false);

			return;
		}
	}

	LoadState = ESSGameModeLoadState::ExecutingActions;

	// Execute the actions
	FGameFeatureActivatingContext Context;

	// Only apply to our specific world context if set
	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	if (ExistingWorldContext)
	{
		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	}

	auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
	{
		for (UGameFeatureAction* Action : ActionList)
		{
			if (Action != nullptr)
			{
				//@TODO: The fact that these don't take a world are potentially problematic in client-server PIE
				// The current behavior matches systems like gameplay tags where loading and registering apply to the entire process,
				// but actually applying the results to actors is restricted to a specific world
				Action->OnGameFeatureRegistering();
				Action->OnGameFeatureLoading();
				Action->OnGameFeatureActivating(Context);
			}
		}
	};

	ActivateListOfActions(CurrentGameMode->Actions);
	for (const TObjectPtr<USSGameModeActionSet>& ActionSet : CurrentGameMode->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}

	LoadState = ESSGameModeLoadState::Loaded;

	OnGameModeLoaded_HighPriority.Broadcast(CurrentGameMode);
	OnGameModeLoaded_HighPriority.Clear();

	OnGameModeLoaded.Broadcast(CurrentGameMode);
	OnGameModeLoaded.Clear();

	OnGameModeLoaded_LowPriority.Broadcast(CurrentGameMode);
	OnGameModeLoaded_LowPriority.Clear();
}

void USSGameModeManagerComponent::OnGameplayReadyDelayed()
{
	SetGameplayReady();
}

void USSGameModeManagerComponent::SetGameplayReady()
{
	bGameplayReady = true;
	
	OnGameModeGameplayReady.Broadcast(this);
	OnGameModeLoaded.Clear();

	UE_LOG(LogSomndusGame, Log, TEXT("EXPERIENCE: Gameplay Ready"));
}

void USSGameModeManagerComponent::SetGameplayReadyDelayed()
{
	GetWorld()->GetTimerManager().SetTimer(GameplayReadyTimerHandle, this, &ThisClass::OnGameplayReadyDelayed,
								   0.1f, /*bLooping=*/ false);
}

void USSGameModeManagerComponent::OnActionDeactivationCompleted()
{
	check(IsInGameThread());
	++NumObservedPausers;

	if (NumObservedPausers == NumExpectedPausers)
	{
		OnAllActionsDeactivated();
	}
}

void USSGameModeManagerComponent::OnAllActionsDeactivated()
{
	//@TODO: We actually only deactivated and didn't fully unload...
	LoadState = ESSGameModeLoadState::Unloaded;
	CurrentGameMode = nullptr;
	//@TODO:	GEngine->ForceGarbageCollection(true);
}

void USSGameModeManagerComponent::SetCurrentGameMode(FPrimaryAssetId ExperienceId)
{
	UAssetManager& AssetManager = UAssetManager::Get();
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	TSubclassOf<USSGameModeDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass);
	const USSGameModeDefinition* Experience = GetDefault<USSGameModeDefinition>(AssetClass);

	check(Experience != nullptr);
	check(CurrentGameMode == nullptr);
	CurrentGameMode = Experience;

	// Check if game module ready or should wait it (especially for development mode for quick testiing)
	CheckGameModuleReady();
}

void USSGameModeManagerComponent::CallOrRegister_OnGameModeLoaded_HighPriority(
	FOnSSGameModeLoaded::FDelegate&& Delegate)
{
	if (IsGameModeLoaded())
	{
		Delegate.Execute(CurrentGameMode);
	}
	else
	{
		OnGameModeLoaded_LowPriority.Add(MoveTemp(Delegate));
	}
}

void USSGameModeManagerComponent::CallOrRegister_OnGameModeLoaded(FOnSSGameModeLoaded::FDelegate&& Delegate)
{
	if (IsGameModeLoaded())
	{
		Delegate.Execute(CurrentGameMode);
	}
	else
	{
		OnGameModeLoaded.Add(MoveTemp(Delegate));
	}
}

void USSGameModeManagerComponent::CallOrRegister_OnGameModeLoaded_LowPriority(FOnSSGameModeLoaded::FDelegate&& Delegate)
{
	if (IsGameModeLoaded())
	{
		Delegate.Execute(CurrentGameMode);
	}
	else
	{
		OnGameModeLoaded_LowPriority.Add(MoveTemp(Delegate));
	}
}

const USSGameModeDefinition* USSGameModeManagerComponent::GetCurrentGameModeChecked() const
{
	check(LoadState == ESSGameModeLoadState::Loaded);
	check(CurrentGameMode != nullptr);
	return CurrentGameMode;
}

bool USSGameModeManagerComponent::IsGameModeLoaded() const
{
	return (LoadState == ESSGameModeLoadState::Loaded) && (CurrentGameMode != nullptr);
}

void USSGameModeManagerComponent::CallOrRegister_OnGameModeGameplayReady(FOnSSGameModeGameplayReady::FDelegate&& Delegate)
{
	if (bGameplayReady)
	{
		Delegate.Execute(this);
	}
	else
	{
		OnGameModeGameplayReady.Add(MoveTemp(Delegate));
	}
}
