// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSGameplayModeManagerComponent.generated.h"


class USSGameModeDefinition;
class USSGameModeManagerComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOMNDUSGAMEMODULAR_API USSGameplayModeManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USSGameplayModeManagerComponent();
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bGameModeIsPlaying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool bUseAutoFadeScreen = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool bSuspendInputWhileLoading = false;
	
	UPROPERTY()
	TObjectPtr<USSGameModeManagerComponent> GameModeManagerComponent;

	UPROPERTY()
	FTimerHandle DelayedOnGameModeLoadedHandle;
	
	void SetGameModeManager(USSGameModeManagerComponent* InGameModeManagerComponent);
	
	bool CheckAllReady(bool bPassLoadingScreenCheck=false);

	void RequestDelayCheck();
	
	void InternalOnGameStateModeReady();
	virtual void OnGameStateModeReady();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGameStateModeReady();
	
	void DelayedOnGameModeLoaded();
	
	UFUNCTION()
	void OnGameModeLoaded(const USSGameModeDefinition* GameModeDefinition);
	
	FDelegateHandle LoadingScreenVisibilityChangedHandle;
	
	UFUNCTION()
	void OnLoadingScreenVisibilityChanged(bool bInVisible);
};
