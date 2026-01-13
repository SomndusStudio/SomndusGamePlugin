// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SSGameHUDLayout.h"
#include "SSGameModularHUDLayout.generated.h"

class USSGameModeManagerComponent;

/**
 * 
 */
UCLASS()
class SOMNDUSGAMEMODULAR_API USSGameModularHUDLayout : public USSGameHUDLayout
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGameModeGameplayReady(USSGameModeManagerComponent* GameModeManagerComponent);
	
	void OnGameModeGameplayReady(USSGameModeManagerComponent* GameModeManagerComponent);
	
	virtual void NativeOnInitialized() override;
};
