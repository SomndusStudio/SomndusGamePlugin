// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SSGameSubsystem.h"

USSGameModule* USSGameSubsystem::GetGameModule(TSubclassOf<USSGameModule> GameModuleClass)
{
	for (auto* GameModule : GameModules)
	{
		if (GameModule)
		{
			return GameModule;
		}
	}

	auto* GameModule = NewObject<USSGameModule>(this, GameModuleClass);
	GameModule->NativeInit();
	GameModules.Add(GameModule);

	return GameModule;
}
