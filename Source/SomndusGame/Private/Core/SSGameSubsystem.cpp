/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Core/SSGameSubsystem.h"

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
