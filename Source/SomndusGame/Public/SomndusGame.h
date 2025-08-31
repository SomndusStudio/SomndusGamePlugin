// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSomndusGameModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
