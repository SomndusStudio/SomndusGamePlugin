// ©2025 Schartier Isaac. All rights reserved.


#include "Mode/SSModularPlayerState.h"

#include "SSLog.h"
#include "Mode/SSCommonModularGameMode.h"
#include "Mode/SSGameModeDefinition.h"
#include "Mode/SSGameModeManagerComponent.h"
#include "Mode/SSPawnExtensionComponent.h"


FName ASSModularPlayerState::NAME_LocalPlayerDataReady = FName("LocalPlayerDataReady");

ASSModularPlayerState::ASSModularPlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void ASSModularPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (auto* PawnExtComp = USSPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}


void ASSModularPlayerState::OnGameModeLoaded(const USSGameModeDefinition* CurrentGameMode)
{
	if (auto* GameMode = GetWorld()->GetAuthGameMode<ASSCommonModularGameMode>())
	{
		if (const auto* NewPawnData = GameMode->GetPawnDataForController(GetOwningController()))
		{
			PawnsData = CurrentGameMode->PawnsData;
			UE_LOG(LogSomndusGame, Log, TEXT("Loaded %d pawns data for the player state"), PawnsData.Num());
			
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogSomndusGame, Error, TEXT("ASSModularPlayerState::OnGameModeLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}

void ASSModularPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		USSGameModeManagerComponent* GameModeManagerComponent = GameState->FindComponentByClass<USSGameModeManagerComponent>();
		check(GameModeManagerComponent);
		GameModeManagerComponent->CallOrRegister_OnGameModeLoaded(FOnSSGameModeLoaded::FDelegate::CreateUObject(this, &ThisClass::OnGameModeLoaded));
	}
}

void ASSModularPlayerState::SetPawnData(const USSPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogSomndusGame, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}
	
	PawnData = InPawnData;
	
	ForceNetUpdate();
}

FGameplayTag ASSModularPlayerState::GetPawnTagFromActor(APawn* InPawn)
{
	for (const auto& PawnDataIt : PawnsData)
	{
		if (InPawn->IsA(PawnDataIt->PawnClass))
		{
			return PawnDataIt->PawnTypeTag;
		}
	}
	return FGameplayTag::EmptyTag;
}