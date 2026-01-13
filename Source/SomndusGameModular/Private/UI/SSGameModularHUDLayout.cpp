// ©2025 Schartier Isaac. All rights reserved.


#include "UI/SSGameModularHUDLayout.h"

#include "Kismet/GameplayStatics.h"
#include "Mode/SSGameModeManagerComponent.h"

void USSGameModularHUDLayout::OnGameModeGameplayReady(USSGameModeManagerComponent* GameModeManagerComponent)
{
	BP_OnGameModeGameplayReady(GameModeManagerComponent);
}

void USSGameModularHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	REGISTER_GAME_MODE_GAMEPLAY_READY_RAW(this, this,&ThisClass::OnGameModeGameplayReady);
}
