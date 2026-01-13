// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
//#include "SSGameModularTypes.generated.h"

namespace SSGameplayTags
{
	SOMNDUSGAMEMODULAR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	SOMNDUSGAMEMODULAR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	SOMNDUSGAMEMODULAR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	SOMNDUSGAMEMODULAR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	SOMNDUSGAMEMODULAR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameMode_Default);
	SOMNDUSGAMEMODULAR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameMode_Common);
}

namespace SSGameModularHelpers
{
	static void SendGameFrameworkComponentExtensionEvent(AActor* Receiver, const FName& EventName, bool bOnlyInGameWorlds = true);
}
