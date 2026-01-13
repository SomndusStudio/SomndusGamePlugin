/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Core/SSGameSubsystem.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Templates/SubclassOf.h"

const FName USSGameSubsystem::LoadStateGameCore(TEXT("GameCore"));

USSGameSubsystem* USSGameSubsystem::Get(const UObject* WorldContextObject)
{
	return UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<USSGameSubsystem>();
}

USSGameModule* USSGameSubsystem::GetGameModule(TSubclassOf<USSGameModule> GameModuleClass)
{
	for (auto* GameModule : GameModules)
	{
		if (GameModule && GameModule->IsA(GameModuleClass))
		{
			return GameModule;
		}
	}

	auto* GameModule = NewObject<USSGameModule>(this, GameModuleClass);
	GameModule->NativeInit();
	GameModules.Add(GameModule);

	return GameModule;
}
