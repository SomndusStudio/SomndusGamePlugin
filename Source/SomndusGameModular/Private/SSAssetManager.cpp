// ©2025 Schartier Isaac. All rights reserved.


#include "SSAssetManager.h"

#include "SSLog.h"
#include "Engine/Engine.h"
#include "Misc/CommandLine.h"
#include "Stats/StatsMisc.h"

const USSPawnData* USSAssetManager::GetDefaultPawnData() const
{
	return GetAsset(DefaultPawnData);
}


UObject* USSAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (IsInitialized())
		{
			return GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool USSAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void USSAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

USSAssetManager& USSAssetManager::GetSSAssetManager()
{
	USSAssetManager* This = Cast<USSAssetManager>(GEngine->AssetManager);
	if (This)
	{
		return *This;
	}
	UE_LOG(LogSomndusGame, Fatal, TEXT("Invalid SS Asset Manager in DefaultEngine.ini"));
	return *NewObject<USSAssetManager>();
}