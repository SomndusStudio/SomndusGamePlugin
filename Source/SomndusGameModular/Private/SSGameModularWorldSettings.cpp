// ©2025 Schartier Isaac. All rights reserved.


#include "SSGameModularWorldSettings.h"


#include "EngineUtils.h"
#include "SSLog.h"
#include "Engine/AssetManager.h"
#include "GameFramework/PlayerStart.h"
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"

#if WITH_EDITOR
void ASSGameModularWorldSettings::CheckForErrors()
{
	Super::CheckForErrors();
	
	FMessageLog MapCheck("MapCheck");

	for (TActorIterator<APlayerStart> PlayerStartIt(GetWorld()); PlayerStartIt; ++PlayerStartIt)
	{
		APlayerStart* PlayerStart = *PlayerStartIt;
		if (IsValid(PlayerStart) && PlayerStart->GetClass() == APlayerStart::StaticClass())
		{
			MapCheck.Warning()
				->AddToken(FUObjectToken::Create(PlayerStart))
				->AddToken(FTextToken::Create(FText::FromString("is a normal APlayerStart, replace with ADUPlayerStart.")));
		}
	}

	//@TODO: Make sure the soft object path is something that can actually be turned into a primary asset ID (e.g., is not pointing to an experience in an unscanned directory)
}
#endif

FPrimaryAssetId ASSGameModularWorldSettings::GetDefaultGameplayMode() const
{
	FPrimaryAssetId Result;
	if (!DefaultGameplayMode.IsNull())
	{
		Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayMode.ToSoftObjectPath());

		if (!Result.IsValid())
		{
			UE_LOG(LogSomndusGame, Error, TEXT("%s.DefaultGameplayMode is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
				*GetPathNameSafe(this), *DefaultGameplayMode.ToString());
		}
	}
	return Result;
}
