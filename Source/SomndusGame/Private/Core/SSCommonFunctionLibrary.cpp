/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Core/SSCommonFunctionLibrary.h"

#include "Core/SSGameSubsystem.h"
#include "DAO/SSDAOManager.h"
#include "Kismet/GameplayStatics.h"

USSGameModule* USSCommonFunctionLibrary::GetGameModule(UObject* ContextObject, TSubclassOf<USSGameModule> Class)
{
	if (USSGameSubsystem* GameSubsystem = UGameplayStatics::GetGameInstance(ContextObject)->GetSubsystem<
	USSGameSubsystem>())
	{
		return GameSubsystem->GetGameModule(Class);
	}
	UE_LOG(LogTemp, Warning, TEXT("Can't found GameModule named %s"), *Class->GetName());
	return nullptr;
}

USSDAOObjectBase* USSCommonFunctionLibrary::GetDAOByClass(UObject* ContextObject, TSubclassOf<USSDAOObjectBase> Class)
{
	if (auto* DAOManager = GetGameModuleByClass<USSDAOManager>(ContextObject))
	{
		return DAOManager->GetDAO(Class);
	}
	UE_LOG(LogTemp, Warning, TEXT("Can't found DAO named %s"), *Class->GetName());
	return nullptr;
}
