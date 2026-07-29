// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/SSGameplayModeManagerComponent.h"

#include "LoadingScreenManager.h"
#include "SSGameModularTypes.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Input/SSInputStaticsLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/SSGameModeManagerComponent.h"
#include "UI/Fade/SSScreenFadeFunctionLibrary.h"
#include "UI/Fade/SSScreenFadeTypes.h"


// Sets default values for this component's properties
USSGameplayModeManagerComponent::USSGameplayModeManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USSGameplayModeManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Suspend player
	if (bSuspendInputWhileLoading)
	{
		USSInputStaticsLibrary::SetGameSuspendInput(this, SSGameplayTags::Tag_Input_Token_GameModeLoading, true);
	}
	
	// Auto capture game mode manager
	if (!GameModeManagerComponent)
	{
		auto* CapturedGameModeManagerComponent = GetOwner()->FindComponentByClass<USSGameModeManagerComponent>();
		SetGameModeManager(CapturedGameModeManagerComponent);
	}
}

void USSGameplayModeManagerComponent::SetGameModeManager(USSGameModeManagerComponent* InGameModeManagerComponent)
{
	GameModeManagerComponent = InGameModeManagerComponent;

	check(GameModeManagerComponent);

	// Bind loading screen
	if (ULoadingScreenManager* LoadingScreenManager = UGameInstance::GetSubsystem<ULoadingScreenManager>(GetWorld()->GetGameInstance()))
	{
		LoadingScreenVisibilityChangedHandle = LoadingScreenManager->OnLoadingScreenVisibilityChangedDelegate().AddUObject(this, &USSGameplayModeManagerComponent::OnLoadingScreenVisibilityChanged);
	}
	
	GameModeManagerComponent->CallOrRegister_OnGameModeLoaded(FOnSSGameModeLoaded::FDelegate::CreateUObject(this, &ThisClass::OnGameModeLoaded));
}

bool USSGameplayModeManagerComponent::CheckAllReady(bool bPassLoadingScreenCheck)
{
	// If loading screen
	if (!bPassLoadingScreenCheck)
	{
		if (ULoadingScreenManager* LoadingScreenManager = UGameInstance::GetSubsystem<ULoadingScreenManager>(GetWorld()->GetGameInstance()))
		{
			if (LoadingScreenManager->GetLoadingScreenDisplayStatus())
			{
				RequestDelayCheck();
				return false;
			}
		}
	}
	
	InternalOnGameStateModeReady();

	return true;
}

void USSGameplayModeManagerComponent::RequestDelayCheck()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayedOnGameModeLoadedHandle);
	GetWorld()->GetTimerManager().SetTimer(
		DelayedOnGameModeLoadedHandle,
		this,
		&USSGameplayModeManagerComponent::DelayedOnGameModeLoaded,
		0.5f,
		false
	);
}

void USSGameplayModeManagerComponent::InternalOnGameStateModeReady()
{
	// Now is playing
	bGameModeIsPlaying = true;

	// Unbind loading screen
	if (ULoadingScreenManager* LoadingScreenManager = UGameInstance::GetSubsystem<ULoadingScreenManager>(GetWorld()->GetGameInstance()))
	{
		LoadingScreenManager->OnLoadingScreenVisibilityChangedDelegate().Remove(LoadingScreenVisibilityChangedHandle);
	}
	
	// Clear
	GetWorld()->GetTimerManager().ClearTimer(DelayedOnGameModeLoadedHandle);
	
	// restore player input
	if (bSuspendInputWhileLoading)
	{
		USSInputStaticsLibrary::SetGameSuspendInput(this, SSGameplayTags::Tag_Input_Token_GameModeLoading, false);
	}

	OnGameStateModeReady();
}

void USSGameplayModeManagerComponent::OnGameStateModeReady()
{
	// Fade in
	if (bUseAutoFadeScreen)
	{
		const FSSScreenFadeDelegate OnFinished;
		const FSSScreenFadeParams FadeParams(0.5f, FLinearColor::Black, FLinearColor::Transparent, OnFinished, true, true);
		USSScreenFadeFunctionLibrary::Fade(this, FadeParams, nullptr, 1);
	}
	
	BP_OnGameStateModeReady();
}

void USSGameplayModeManagerComponent::DelayedOnGameModeLoaded()
{
	CheckAllReady();
}

void USSGameplayModeManagerComponent::OnGameModeLoaded(const USSGameModeDefinition* GameModeDefinition)
{
	CheckAllReady();
}

void USSGameplayModeManagerComponent::OnLoadingScreenVisibilityChanged(bool bInVisible)
{
	if (!bInVisible)
	{
		if (bGameModeIsPlaying) return;

		CheckAllReady(true);
	}
}
