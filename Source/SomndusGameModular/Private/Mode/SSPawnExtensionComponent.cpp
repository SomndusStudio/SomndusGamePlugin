// ©2025 Schartier Isaac. All rights reserved.


#include "Mode/SSPawnExtensionComponent.h"


#include "SSGameModularTypes.h"
#include "SSLog.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Core/SSGameSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

const FName USSPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

USSPawnExtensionComponent::USSPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
}

void USSPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USSPawnExtensionComponent, PawnData);
}

void USSPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("DUPawnExtensionComponent on [%s] can only be added to Pawn actors."),
	                 *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(USSPawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1),
	                 TEXT("Only one DUPawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));

	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

// Called when the game starts
void USSPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for changes to all features
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// Notifies state manager that we have spawned, then try rest of default initialization
	ensure(TryToChangeInitState(SSGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void USSPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

bool USSPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                                   FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == SSGameplayTags::InitState_Spawned)
	{
		// As long as we are on a valid pawn, we count as spawned
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == SSGameplayTags::InitState_Spawned && DesiredState == SSGameplayTags::InitState_DataAvailable)
	{
		// Pawn data is required.
		if (!PawnData)
		{
			return false;
		}

		// The player state is required if it's a player state
		if (!GetPlayerState<APlayerState>())
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		if (bHasAuthority || bIsLocallyControlled)
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
			{
				return false;
			}
		}
		
		return true;
	}
	else if (CurrentState == SSGameplayTags::InitState_DataAvailable && DesiredState ==
		SSGameplayTags::InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		bool bPawnComponentsInitialized = Manager->HaveAllFeaturesReachedInitState(
			Pawn, SSGameplayTags::InitState_DataAvailable);

		// TODO : Maybe change this part to be no dependant of player state actor components
		// In wait should check only if the player state have at least on actor component with states chain
		auto* PlayerState = GetPlayerState<APlayerState>();
		if (auto* GameFrameworkInitStateInterface = PlayerState->FindComponentByInterface<
			UGameFrameworkInitStateInterface>())
		{
			bool bPlayerStateComponentsInitialized = Manager->HaveAllFeaturesReachedInitState(
				PlayerState, SSGameplayTags::InitState_DataAvailable);

			return bPawnComponentsInitialized && bPlayerStateComponentsInitialized;
		}

		return bPawnComponentsInitialized;
	}
	else if (CurrentState == SSGameplayTags::InitState_DataInitialized && DesiredState ==
		SSGameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void USSPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager,
                                                      FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (DesiredState == SSGameplayTags::InitState_DataAvailable)
	{
		SSGameModularHelpers::SendGameFrameworkComponentExtensionEvent(GetPlayerState<APlayerState>(),
		                                                           "PawnExtension_DataAvailable");
	}
	if (DesiredState == SSGameplayTags::InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change
	}
	if (DesiredState == SSGameplayTags::InitState_GameplayReady)
	{
		// This is currently all handled by other components listening to this state change
		SSGameModularHelpers::SendGameFrameworkComponentExtensionEvent(GetPlayerState<APlayerState>(), "PawnExtension_GameplayReady");
	}
}

void USSPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == SSGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void USSPawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = {
		SSGameplayTags::InitState_Spawned, SSGameplayTags::InitState_DataAvailable,
		SSGameplayTags::InitState_DataInitialized, SSGameplayTags::InitState_GameplayReady
	};

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void USSPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

void USSPawnExtensionComponent::SetPawnData(const USSPawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogSomndusGame, Error,
		       TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."),
		       *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
}

void USSPawnExtensionComponent::HandleControllerChanged()
{
	CheckDefaultInitialization();
}

void USSPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}
