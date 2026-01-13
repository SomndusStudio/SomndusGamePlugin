// ©2025 Schartier Isaac. All rights reserved.


#include "Mode/SSCommonModularGameMode.h"


#include "SSAssetManager.h"
#include "SSGameModularWorldSettings.h"
#include "SSLog.h"
#include "TimerManager.h"
#include "AssetRegistry/AssetData.h"
#include "Engine/AssetManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/SSGameModeDefinition.h"
#include "Mode/SSGameModeManagerComponent.h"
#include "Mode/SSModularPlayerState.h"
#include "Mode/SSPawnExtensionComponent.h"
#include "Pawn/SSPawnData.h"

ASSCommonModularGameMode::ASSCommonModularGameMode(const FObjectInitializer& ObjectInitializer)
{
	//GameStateClass = ADUCommonGameState::StaticClass();
	//PlayerControllerClass = ADUPlayerController::StaticClass();
	PlayerStateClass = ASSModularPlayerState::StaticClass();
	
	bUseSeamlessTravel = true;
}

ASSCommonModularGameMode* ASSCommonModularGameMode::Get(const UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->GetAuthGameMode<ASSCommonModularGameMode>();
}


void ASSCommonModularGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	// Wait for the next frame to give time to initialize startup settings
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ASSCommonModularGameMode::InitGameState()
{
	Super::InitGameState();
	
	// Listen for the experience load to complete	
	USSGameModeManagerComponent* GameModeComponent = GameState->FindComponentByClass<USSGameModeManagerComponent>();
	check(GameModeComponent);
	GameModeComponent->CallOrRegister_OnGameModeLoaded(FOnSSGameModeLoaded::FDelegate::CreateUObject(this, &ThisClass::OnGameModeLoaded));
}

UClass* ASSCommonModularGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const auto* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ASSCommonModularGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
                                                                     const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (auto* PawnExtComp = USSPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const auto* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogSomndusGame, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogSomndusGame, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogSomndusGame, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

void ASSCommonModularGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Spawn player only when the game mode it's loaded to avoid unsychonized event
	if (IsGameModeLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

const USSPawnData* ASSCommonModularGameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const auto* ModularPlayerState = InController->GetPlayerState<ASSModularPlayerState>())
		{
			if (const auto* PawnData = ModularPlayerState->GetPawnData<USSPawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	auto* GameModeComponent = GameState->FindComponentByClass<USSGameModeManagerComponent>();
	check(GameModeComponent);

	if (GameModeComponent->IsGameModeLoaded())
	{
		const auto* GameMode = GameModeComponent->GetCurrentGameModeChecked();
		if (auto DefaultPawnData = GameMode->GetDefaultPawnData())
		{
			return DefaultPawnData;
		}
		/*
		if (!GameMode->DefaultPawnData.IsNull())
		{
			return GameMode->DefaultPawnData.LoadSynchronous();
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		*/
		return USSAssetManager::GetSSAssetManager().GetDefaultPawnData();
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}

void ASSCommonModularGameMode::OnGameModeLoaded(const USSGameModeDefinition* CurrentGameMode)
{
	// Spawn any players that are already attached
	//@TODO: Here we're handling only *player* controllers, but in GetDefaultPawnClassForController_Implementation we skipped all controllers
	// GetDefaultPawnClassForController_Implementation might only be getting called for players anyways
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool ASSCommonModularGameMode::IsGameModeLoaded() const
{
	check(GameState);
	auto* GameModeComponent = GameState->FindComponentByClass<USSGameModeManagerComponent>();
	check(GameModeComponent);

	return GameModeComponent->IsGameModeLoaded();
}

void ASSCommonModularGameMode::OnMatchAssignmentGiven(FPrimaryAssetId GameModeId, const FString& GameModeIdSource)
{
	if (GameModeId.IsValid())
	{
		UE_LOG(LogSomndusGame, Log, TEXT("Identified experience %s (Source: %s)"), *GameModeId.ToString(), *GameModeIdSource);

		auto* GameModeComponent = GameState->FindComponentByClass<USSGameModeManagerComponent>();
		check(GameModeComponent);
		GameModeComponent->SetCurrentGameMode(GameModeId);
	}
	else
	{
		UE_LOG(LogSomndusGame, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}

void ASSCommonModularGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId GameModeId;
	FString GameModeIdSource;

	// Precedence order (highest wins)
	//  - Matchmaking assignment (if present)
	//  - URL Options override
	//  - Developer Settings (PIE only)
	//  - Command Line override
	//  - World Settings
	//  - Dedicated server
	//  - Default experience

	UWorld* World = GetWorld();
	
	// see if the world settings has a default experience
	if (!GameModeId.IsValid())
	{
		if (const auto* TypedWorldSettings = Cast<ASSGameModularWorldSettings>(GetWorldSettings()))
		{
			GameModeId = TypedWorldSettings->GetDefaultGameplayMode();
			GameModeIdSource = TEXT("WorldSettings");
		}
	}

	UAssetManager& AssetManager = UAssetManager::Get();
	FAssetData Dummy;
	if (GameModeId.IsValid() && !AssetManager.GetPrimaryAssetData(GameModeId, /*out*/ Dummy))
	{
		UE_LOG(LogSomndusGame, Error, TEXT("GAME MODE: Wanted to use %s but couldn't find it, falling back to the default)"), *GameModeId.ToString());
		GameModeId = FPrimaryAssetId();
	}

	// Final fallback to the default experience
	/*
	if (!GameModeId.IsValid())
	{
		if (TryDedicatedServerLogin())
		{
			// This will start to host as a dedicated server
			return;
		}

		//@TODO: Pull this from a config setting or something 
		GameModeId = FPrimaryAssetId(FPrimaryAssetType("LyraExperienceDefinition"), FName("B_LyraDefaultExperience"));
		GameModeIdSource = TEXT("Default");
	}
	*/
	OnMatchAssignmentGiven(GameModeId, GameModeIdSource);
}
