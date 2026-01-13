// ©2025 Schartier Isaac. All rights reserved.


#include "SSGameModularTypes.h"

#include "Components/GameFrameworkComponentManager.h"

namespace SSGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "1: Actor/component has initially spawned and can be extended");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");
	
	UE_DEFINE_GAMEPLAY_TAG(GameMode_Default, "GameMode.Default");
	UE_DEFINE_GAMEPLAY_TAG(GameMode_Common, "GameMode.Common");
}

void SSGameModularHelpers::SendGameFrameworkComponentExtensionEvent(AActor* Receiver, const FName& EventName,
	bool bOnlyInGameWorlds)
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(Receiver, EventName, bOnlyInGameWorlds);
}
